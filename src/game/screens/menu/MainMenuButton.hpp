#include <Geode/cocos/include/cocos2d.h>
#include "ButtonSystemState.hpp"
#include "ButtonSystem.hpp"
#include "ButtonArea.hpp"
#include "../../graphics/containers/BeatSyncedContainer.hpp"
#include "../../../framework/graphics/Color4.hpp"

using namespace cocos2d;

enum ButtonState
{
    Contracted,
    Expanded,
    Exploded
};
/// <summary>
/// Button designed specifically for the osu!next main menu.
/// In order to correctly flow, we have to use a negative margin on the parent container (due to the parallelogram shape).
/// </summary>
class MainMenuButton : public CCMenuItem, BeatSyncedContainer
{
    float const BOUNCE_COMPRESSION = 0.9f;
    float const HOVER_SCALE = 1.2f;
    float const BOUNCE_ROTATION = 8;
/*
    public readonly Key[] TriggerKeys;

    protected override Container<Drawable> Content => content;
    private readonly Container content;
    */

    CCNode* content;

    /// <summary>
    /// The menu state for which we are visible for (assuming only one).
    /// </summary>
  public:
    /*
    ButtonSystemState VisibleState
    {
        set
        {
            VisibleStateMin = value;
            VisibleStateMax = value;
        }
    }
    */

    ButtonSystemState VisibleStateMin = ButtonSystemState::TopLevel;
    ButtonSystemState VisibleStateMax = ButtonSystemState::TopLevel;

    /*
    public new MarginPadding Padding
    {
        get => Content.Padding;
        set => Content.Padding = value;
    }
    */

  protected:
    CCSize BaseSize = CCSize(ButtonSystem.BUTTON_WIDTH, ButtonArea.BUTTON_AREA_HEIGHT);

  private:
    SEL_MenuHandler clickAction;

    CCNode* background;
    CCNode* backgroundContent;
    CCNode* boxHoverLayer; // ?
    CCSprite* icon;

    CCSize initialSize = BaseSize + content.getContentSize();

    std::string sampleName;
    char* sampleClick;
    const char* sampleHover = "j.wav";

    /*
    public override bool IsPresent => base.IsPresent
                                      // Allow keyboard interaction based on state rather than waiting for delayed animations.
                                      || state == ButtonState.Expanded;

    public override bool ReceivePositionalInputAt(Vector2 screenSpacePos) => background.ReceivePositionalInputAt(screenSpacePos);
*/
    static MainMenuButton* create(std::string text, std::string sampleName, CCSprite* symbol, Color4 colour, SEL_MenuHandler clickAction);
    bool init(std::string text, std::string sampleName, CCSprite* symbol, Color4 colour, SEL_MenuHandler clickAction);

  protected:
    virtual CCNode* createBackground(Color4 accentColour) 
    {
      /*
        Child = new Box
        {
            EdgeSmoothness = new Vector2(1.5f, 0),
            RelativeSizeAxes = Axes.Both,
            Colour = accentColour,
        
        }
      */
      return CCNode::create();
    };

    void loadComplete()
    {
        //base.LoadComplete();

        background.Shear = new Vector2(ButtonSystem.WEDGE_WIDTH / initialSize.Y, 0);

        // for whatever reason, attempting to size the background "just in time" to cover the visible width
        // results in gaps when the width changes are quick (only visible when testing menu at 100% speed, not visible slowed down).
        // to ensure there's no missing backdrop, just use a ballpark that should be enough to always cover the width and then some.
        // note that while on a code inspections it would seem that `1.5 * initialSize.X` would be enough, elastic usings are used in this button
        // (which can exceed the [0;1] range during interpolation).
        backgroundContent.Width = 2 * initialSize.X;
        backgroundContent.Shear = -background.Shear;

        animateState();
        FinishTransforms(true);
    }

  private:
    bool rightward;

  protected:
    void onBeat(float elapsed)
    {
        if (!isHovered) return;

        double duration = timingPoint.BeatLength / 2;

        icon.RotateTo(rightward ? BOUNCE_ROTATION : -BOUNCE_ROTATION, duration * 2, Easing.InOutSine);

        icon.Animate(
            i => i.MoveToY(-10, duration, Easing.Out),
            i => i.ScaleTo(HOVER_SCALE, duration, Easing.Out)
        ).Then(
            i => i.MoveToY(0, duration, Easing.In),
            i => i.ScaleTo(new Vector2(HOVER_SCALE, HOVER_SCALE * BOUNCE_COMPRESSION), duration, Easing.In)
        );

        rightward = !rightward;
    }
    bool onHover()
    {
        if (State != ButtonState.Expanded) return true;

        double duration = TimeUntilNextBeat;

        icon.ClearTransforms();
        icon.RotateTo(rightward ? -BOUNCE_ROTATION : BOUNCE_ROTATION, duration, Easing.InOutSine);
        icon.ScaleTo(new Vector2(HOVER_SCALE, HOVER_SCALE * BOUNCE_COMPRESSION), duration, Easing.Out);

        sampleHover?.Play();
        background.ResizeTo(Vector2.Multiply(initialSize, new Vector2(1.5f, 1)), 500, Easing.OutElastic);

        return true;
    }

    protected override void OnHoverLost(HoverLostEvent e)
    {
        icon.ClearTransforms();
        icon.RotateTo(0, 500, Easing.Out);
        icon.MoveTo(Vector2.Zero, 500, Easing.Out);
        icon.ScaleTo(Vector2.One, 200, Easing.Out);

        if (State == ButtonState.Expanded)
            background.ResizeTo(initialSize, 500, Easing.OutElastic);
    }

    [BackgroundDependencyLoader]
    private void load(AudioManager audio)
    {
        sampleHover = audio.Samples.Get(@"Menu/button-hover");
        sampleClick = audio.Samples.Get(!string.IsNullOrEmpty(sampleName) ? $@"Menu/{sampleName}" : @"UI/button-select");
    }

    protected override bool OnMouseDown(MouseDownEvent e)
    {
        boxHoverLayer.FadeTo(0.1f, 1000, Easing.OutQuint);
        return base.OnMouseDown(e);
    }

    protected override void OnMouseUp(MouseUpEvent e)
    {
        boxHoverLayer.FadeTo(0, 1000, Easing.OutQuint);
        base.OnMouseUp(e);
    }

    protected override bool OnClick(ClickEvent e)
    {
        trigger();
        return true;
    }

    protected override bool OnKeyDown(KeyDownEvent e)
    {
        if (e.Repeat || e.ControlPressed || e.ShiftPressed || e.AltPressed || e.SuperPressed)
            return false;

        if (TriggerKeys.Contains(e.Key))
        {
            trigger();
            return true;
        }

        return false;
    }

    private void trigger()
    {
        sampleChannel = sampleClick?.GetChannel();
        sampleChannel?.Play();

        clickAction?.Invoke(this);

        boxHoverLayer.ClearTransforms();
        boxHoverLayer.Alpha = 0.9f;
        boxHoverLayer.FadeOut(800, Easing.OutExpo);
    }

    public override bool HandleNonPositionalInput => state == ButtonState.Expanded;
    public override bool HandlePositionalInput => state != ButtonState.Exploded && background.Width / initialSize.X >= 0.8f;

    public void StopSamplePlayback() => sampleChannel?.Stop();

    protected override void Update()
    {
        content.Alpha = Math.Clamp((background.Width / initialSize.X - 0.5f) / 0.3f, 0, 1);
        base.Update();
    }

    public int ContractStyle;

    private ButtonState state;

    public ButtonState State
    {
        get => state;

        set
        {
            if (state == value)
                return;

            state = value;

            animateState();

            StateChanged?.Invoke(State);
        }
    }

    private void animateState()
    {
        switch (state)
        {
            case ButtonState.Contracted:
                switch (ContractStyle)
                {
                    default:
                        background.ResizeTo(Vector2.Multiply(initialSize, new Vector2(0, 1)), 500, Easing.OutExpo);
                        this.FadeOut(500);
                        break;

                    case 1:
                        background.ResizeTo(Vector2.Multiply(initialSize, new Vector2(0, 1)), 400, Easing.InSine);
                        this.FadeOut(800);
                        break;
                }

                break;

            case ButtonState.Expanded:
                const int expand_duration = 500;
                background.ResizeTo(initialSize, expand_duration, Easing.OutExpo);
                this.FadeIn(expand_duration / 6f);
                break;

            case ButtonState.Exploded:
                const int explode_duration = 200;
                background.ResizeTo(Vector2.Multiply(initialSize, new Vector2(2, 1)), explode_duration, Easing.OutExpo);
                this.FadeOut(explode_duration / 4f * 3);
                break;
        }
    }

    private ButtonSystemState buttonSystemState;

    public ButtonSystemState ButtonSystemState
    {
        get => buttonSystemState;
        set
        {
            if (buttonSystemState == value)
                return;

            buttonSystemState = value;
            UpdateState();
        }
    }

    protected virtual void UpdateState()
    {
        ContractStyle = 0;

        switch (ButtonSystemState)
        {
            case ButtonSystemState.Initial:
                State = ButtonState.Contracted;
                break;

            case ButtonSystemState.EnteringMode:
                ContractStyle = 1;
                State = ButtonState.Contracted;
                break;

            default:
                if (ButtonSystemState <= VisibleStateMax && ButtonSystemState >= VisibleStateMin)
                    State = ButtonState.Expanded;
                else if (ButtonSystemState < VisibleStateMin)
                    State = ButtonState.Contracted;
                else
                    State = ButtonState.Exploded;
                break;
        }
    }
}
