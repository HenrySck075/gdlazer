#pragma once
#include <Geode/Geode.hpp>
#include <vector>
#include "ButtonArea.hpp"
using namespace geode::prelude;

using OsuLogo = CCNode*;

class ButtonSystem : public CCMenu
{
  public:
    float WEDGE_WIDTH = 20;

    //Action<ButtonSystemState>? StateChanged;

    SEL_MenuHandler onEditBeatmap;
    SEL_MenuHandler onEditSkin;
    SEL_MenuHandler onExit;
    SEL_MenuHandler onBeatmapListing;
    SEL_MenuHandler onSolo;
    SEL_MenuHandler onSettings;
    SEL_MenuHandler onMultiplayer;
    SEL_MenuHandler onPlaylists;
    SEL_MenuHandler onDailyChallenge;

  private:
    OsuLogo logo;

  public:
    /// <summary>
    /// Assign the <see cref="OsuLogo"/> that this ButtonSystem should manage the position of.
    /// </summary>
    /// <param name="logo">The instance of the logo to be assigned. If null, we are suspending from the screen that uses this ButtonSystem.</param>
    void setOsuLogo(OsuLogo logo); 

  private:
    ButtonArea buttonArea;

    /*priv read*/ MainMenuButton backButton;

    /*priv read*/ std::vector<MainMenuButton> buttonsTopLevel = {};
    /*priv read*/ std::vector<MainMenuButton> buttonsPlay = {};
    /*priv read*/ std::vector<MainMenuButton> buttonsEdit = {};

    const char* sampleBackToLogo = "idk.wav";
    const char* sampleLogoSwoosh = "me too.wav";

    /*priv read*/ LogoTrackingContainer logoTrackingContainer;

    ReturnToTopOnIdle { get; set; } = true;

    public ButtonSystem()
    {
        RelativeSizeAxes = Axes.Both;

        Child = logoTrackingContainer = new LogoTrackingContainer
        {
            RelativeSizeAxes = Axes.Both,
            Child = buttonArea = new ButtonArea()
        };

        buttonArea.AddRange(new Drawable[]
        {
            new MainMenuButton(ButtonSystemStrings.Settings, string.Empty, OsuIcon.Settings, new Color4(85, 85, 85, 255), _ => OnSettings?.Invoke(), Key.O, Key.S)
            {
                Padding = new MarginPadding { Right = WEDGE_WIDTH },
            },
            backButton = new MainMenuButton(ButtonSystemStrings.Back, @"back-to-top", OsuIcon.PrevCircle, new Color4(51, 58, 94, 255), _ => State = ButtonSystemState.TopLevel)
            {
                Padding = new MarginPadding { Right = WEDGE_WIDTH },
                VisibleStateMin = ButtonSystemState.Play,
                VisibleStateMax = ButtonSystemState.Edit,
            },
            logoTrackingContainer.LogoFacade.With(d => d.Scale = new Vector2(0.74f))
        });

        buttonArea.Flow.CentreTarget = logoTrackingContainer.LogoFacade;
    }

    [Resolved]
    private IAPIProvider api { get; set; } = null!;

    [Resolved]
    private OsuGame? game { get; set; }

    [Resolved]
    private LoginOverlay? loginOverlay { get; set; }

    [BackgroundDependencyLoader]
    private void load(AudioManager audio, IdleTracker? idleTracker, GameHost host)
    {
        buttonsPlay.Add(new MainMenuButton(ButtonSystemStrings.Solo, @"button-default-select", OsuIcon.Player, new Color4(102, 68, 204, 255), _ => OnSolo?.Invoke(), Key.P)
        {
            Padding = new MarginPadding { Left = WEDGE_WIDTH },
        });
        buttonsPlay.Add(new MainMenuButton(ButtonSystemStrings.Multi, @"button-default-select", OsuIcon.Online, new Color4(94, 63, 186, 255), onMultiplayer, Key.M));
        buttonsPlay.Add(new MainMenuButton(ButtonSystemStrings.Playlists, @"button-default-select", OsuIcon.Tournament, new Color4(94, 63, 186, 255), onPlaylists, Key.L));
        buttonsPlay.Add(new DailyChallengeButton(@"button-default-select", new Color4(94, 63, 186, 255), onDailyChallenge, Key.D));
        buttonsPlay.ForEach(b => b.VisibleState = ButtonSystemState.Play);

        buttonsEdit.Add(new MainMenuButton(EditorStrings.BeatmapEditor.ToLower(), @"button-default-select", OsuIcon.Beatmap, new Color4(238, 170, 0, 255), _ => OnEditBeatmap?.Invoke(), Key.B, Key.E)
        {
            Padding = new MarginPadding { Left = WEDGE_WIDTH },
        });
        buttonsEdit.Add(new MainMenuButton(SkinEditorStrings.SkinEditor.ToLower(), @"button-default-select", OsuIcon.SkinB, new Color4(220, 160, 0, 255), _ => OnEditSkin?.Invoke(), Key.S));
        buttonsEdit.ForEach(b => b.VisibleState = ButtonSystemState.Edit);

        buttonsTopLevel.Add(new MainMenuButton(ButtonSystemStrings.Play, @"button-play-select", OsuIcon.Logo, new Color4(102, 68, 204, 255), _ => State = ButtonSystemState.Play, Key.P, Key.M, Key.L)
        {
            Padding = new MarginPadding { Left = WEDGE_WIDTH },
        });
        buttonsTopLevel.Add(new MainMenuButton(ButtonSystemStrings.Edit, @"button-play-select", OsuIcon.EditCircle, new Color4(238, 170, 0, 255), _ => State = ButtonSystemState.Edit, Key.E));
        buttonsTopLevel.Add(new MainMenuButton(ButtonSystemStrings.Browse, @"button-default-select", OsuIcon.Beatmap, new Color4(165, 204, 0, 255), _ => OnBeatmapListing?.Invoke(), Key.B, Key.D));

        if (host.CanExit)
            buttonsTopLevel.Add(new MainMenuButton(ButtonSystemStrings.Exit, string.Empty, OsuIcon.CrossCircle, new Color4(238, 51, 153, 255), _ => OnExit?.Invoke(), Key.Q));

        buttonArea.AddRange(buttonsPlay);
        buttonArea.AddRange(buttonsEdit);
        buttonArea.AddRange(buttonsTopLevel);

        buttonArea.ForEach(b =>
        {
            if (b is MainMenuButton)
            {
                b.Origin = Anchor.CentreLeft;
                b.Anchor = Anchor.CentreLeft;
            }
        });

        isIdle.ValueChanged += idle => updateIdleState(idle.NewValue);

        if (idleTracker != null) isIdle.BindTo(idleTracker.IsIdle);

        sampleBackToLogo = audio.Samples.Get(@"Menu/back-to-logo");
        sampleLogoSwoosh = audio.Samples.Get(@"Menu/osu-logo-swoosh");
    }

    private void onMultiplayer(MainMenuButton _)
    {
        if (api.State.Value != APIState.Online)
        {
            loginOverlay?.Show();
            return;
        }

        OnMultiplayer?.Invoke();
    }

    private void onPlaylists(MainMenuButton _)
    {
        if (api.State.Value != APIState.Online)
        {
            loginOverlay?.Show();
            return;
        }

        OnPlaylists?.Invoke();
    }

    private void onDailyChallenge(MainMenuButton button)
    {
        if (api.State.Value != APIState.Online)
        {
            loginOverlay?.Show();
            return;
        }

        var dailyChallengeButton = (DailyChallengeButton)button;

        if (dailyChallengeButton.Room != null)
            OnDailyChallenge?.Invoke(dailyChallengeButton.Room);
    }

    private void updateIdleState(bool isIdle)
    {
        if (!ReturnToTopOnIdle)
            return;

        if (isIdle && State != ButtonSystemState.Exit && State != ButtonSystemState.EnteringMode)
            State = ButtonSystemState.Initial;
    }

    /// <summary>
    /// Triggers the <see cref="logo"/> if the current <see cref="State"/> is <see cref="ButtonSystemState.Initial"/>.
    /// </summary>
    /// <returns><c>true</c> if the <see cref="logo"/> was triggered, <c>false</c> otherwise.</returns>
    private bool triggerInitialOsuLogo()
    {
        if (State == ButtonSystemState.Initial)
        {
            StopSamplePlayback();
            logo?.TriggerClick();
            return true;
        }

        return false;
    }

    protected override bool OnKeyDown(KeyDownEvent e)
    {
        if (e.Repeat || e.ControlPressed || e.ShiftPressed || e.AltPressed || e.SuperPressed)
            return false;

        if (triggerInitialOsuLogo())
            return true;

        return base.OnKeyDown(e);
    }

    protected override bool OnJoystickPress(JoystickPressEvent e)
    {
        if (triggerInitialOsuLogo())
            return true;

        return base.OnJoystickPress(e);
    }

    protected override bool OnMidiDown(MidiDownEvent e)
    {
        if (triggerInitialOsuLogo())
            return true;

        return base.OnMidiDown(e);
    }

    OnPressed(KeyBindingPressEvent<GlobalAction> e)
    {
        if (e.Repeat)
            return false;

        switch (e.Action)
        {
            case GlobalAction.Back:
                return goBack();

            case GlobalAction.Select:
                logo?.TriggerClick();
                return true;

            default:
                return false;
        }
    }

    OnReleased(KeyBindingReleaseEvent<GlobalAction> e)
    {
    }

    private bool goBack()
    {
        switch (State)
        {
            case ButtonSystemState.TopLevel:
                State = ButtonSystemState.Initial;

                // Samples are explicitly played here in response to user interaction and not when transitioning due to idle.
                StopSamplePlayback();
                sampleBackToLogo?.Play();

                return true;

            case ButtonSystemState.Edit:
            case ButtonSystemState.Play:
                StopSamplePlayback();
                backButton.TriggerClick();
                return true;

            default:
                return false;
        }
    }

    StopSamplePlayback()
    {
        buttonsPlay.ForEach(button => button.StopSamplePlayback());
        buttonsTopLevel.ForEach(button => button.StopSamplePlayback());
        logo?.StopSamplePlayback();
    }

    private bool onOsuLogo()
    {
        switch (state)
        {
            default:
                return false;

            case ButtonSystemState.Initial:
                State = ButtonSystemState.TopLevel;
                return true;

            case ButtonSystemState.TopLevel:
                buttonsTopLevel.First().TriggerClick();
                return false;

            case ButtonSystemState.Play:
                buttonsPlay.First().TriggerClick();
                return false;

            case ButtonSystemState.Edit:
                buttonsEdit.First().TriggerClick();
                return false;
        }
    }

    private ButtonSystemState state = ButtonSystemState.Initial;

    bool HandleNonPositionalInput => state != ButtonSystemState.Exit;
    bool HandlePositionalInput => state != ButtonSystemState.Exit;

    State
    {
        get => state;

        set
        {
            if (state == value) return;

            ButtonSystemState lastState = state;
            state = value;

            updateLogoState(lastState);

            Logger.Log($"{nameof(ButtonSystem)}'s state changed from {lastState} to {state}");

            buttonArea.FinishTransforms(true);

            using (buttonArea.BeginDelayedSequence(lastState == ButtonSystemState.Initial ? 150 : 0))
            {
                buttonArea.ButtonSystemState = state;

                foreach (var b in buttonArea.OfType<MainMenuButton>())
                    b.ButtonSystemState = state;
            }

            StateChanged?.Invoke(State);
        }
    }

    private ScheduledDelegate? logoDelayedAction;

    private void updateLogoState(ButtonSystemState lastState = ButtonSystemState.Initial)
    {
        if (logo == null) return;

        switch (state)
        {
            case ButtonSystemState.Exit:
            case ButtonSystemState.Initial:
                logoDelayedAction?.Cancel();
                logoDelayedAction = Scheduler.AddDelayed(() =>
                {
                    logoTrackingContainer.StopTracking();

                    game?.Toolbar.Hide();

                    logo?.ClearTransforms(targetMember: nameof(Position));
                    logo?.MoveTo(new Vector2(0.5f), 800, Easing.OutExpo);
                    logo?.ScaleTo(1, 800, Easing.OutExpo);
                }, buttonArea.Alpha * 150);

                if (lastState == ButtonSystemState.TopLevel)
                    sampleLogoSwoosh?.Play();
                break;

            case ButtonSystemState.TopLevel:
            case ButtonSystemState.Play:
                switch (lastState)
                {
                    case ButtonSystemState.TopLevel: // coming from toplevel to play
                        break;

                    case ButtonSystemState.Initial:
                        logo.ClearTransforms(targetMember: nameof(Position));

                        bool impact = logo.Scale.X > 0.6f;

                        logo.ScaleTo(0.5f, 200, Easing.In);

                        logoTrackingContainer.StartTracking(logo, 200, Easing.In);

                        logoDelayedAction?.Cancel();
                        logoDelayedAction = Scheduler.AddDelayed(() =>
                        {
                            if (impact)
                                logo?.Impact();

                            game?.Toolbar.Show();
                        }, 200);
                        break;

                    default:
                        logo.ClearTransforms(targetMember: nameof(Position));
                        logoTrackingContainer.StartTracking(logo, 0, Easing.In);
                        logo.ScaleTo(0.5f, 200, Easing.OutQuint);
                        break;
                }

                break;

            case ButtonSystemState.EnteringMode:
                logoTrackingContainer.StartTracking(logo, lastState == ButtonSystemState.Initial ? MainMenu.FADE_OUT_DURATION : 0, Easing.InSine);
                break;
        }
    }
};

