#include "GridContainer.hpp"

float calcSize(float size, float s, Dimension const& dim) {
    if (dim.mode == GridSizeMode::Relative) return s*size;
    return s;
} 


std::vector<float> getCellSizes(
    CCNode* on, float size, std::vector<Dimension>& dimensions
) {
    int idx = 0;
    std::vector<float> SizeF;
    std::vector<int> Queue;
    std::vector<int> AutoSizeQueue;
    int numItems = dimensions.size();
    SizeF.reserve(numItems);
    // essentially copying
    float remainingSize = 0+size;

    for (;idx<numItems;idx++) {
        auto dim = dimensions[idx];
        if (
            dim.mode == GridSizeMode::Absolute 
            || 
            dim.mode == GridSizeMode::Relative
        ) {
            auto s = calcSize(
                size, std::max((float)dim.size, (float)dim.minSize), dim
            );
            // might be better to just let it take the entire space, 
            // but i didnt support multicell allocation yet,
            // nor did the osu!framework implementation

            // nvm i think they did autosize mode exists

            // nvm you dont have a copy of a coldim every rowdims
            if (dim.maxSize != (float)-1) s = std::min(s, (float)dim.maxSize);
            SizeF[idx] = s;
            remainingSize -= s;
        } else {
            log::debug("[getCellSizes (GridLayout)]: crazy");
            if (dim.mode == GridSizeMode::AutoSize) {
                AutoSizeQueue.push_back(idx);
            }
            else {
                Queue.push_back(idx);
            }
        }
    }
    auto qcount = Queue.size();
    auto aqcount = AutoSizeQueue.size();
    for (auto idx : Queue) {
        auto dim = dimensions[idx];
        auto divSize = remainingSize/qcount;
        float s;
        if (aqcount == 0) {
            s = std::max(divSize, (float)dim.minSize);
            if (dim.maxSize != (float)-1) s = std::min(s, (float)dim.maxSize);
        } 
        // give em a slot
        else {
            s = dim.minSize;
        }
        SizeF[idx] = s;
        remainingSize -= s;
        qcount--;
    }
    if (aqcount != 0) {    
        float maxSize = remainingSize / aqcount;
        for (auto dim : AutoSizeQueue) {SizeF[dim] = maxSize;}
    }
    return SizeF;

}

void GridLayout::apply(CCNode* on) {
    auto nodes = CCArrayExt<CCNode*>(getNodesToPosition(on));

    int colidx = 0;
    int rowidx = 0;

    int numcols = colDim.size();
    int numrows = rowDim.size();
    int numchilds = nodes.inner()->count();

    auto nodeSize = on->CCNode::getContentSize();
    CCSize cellSize {nodeSize.width/numcols, nodeSize.height/numrows};

    // setup
    for (auto* c : nodes) {
        c->ignoreAnchorPointForPosition(false);
        c->setAnchorPoint({0,1});
        if (auto cc = typeinfo_cast<Container*>(c)) {
            cc->setAnchor(Anchor::TopLeft);
            cc->setSizeUnit(Unit::OpenGL, Unit::OpenGL);
            cc->setPositionUnit(Unit::OpenGL, Unit::OpenGL);
        }
        colidx++;
        if (colidx==numcols) {colidx = 0; rowidx++;}
    }

    colidx = 0; rowidx = 0;
    
    auto colSize = getCellSizes(on, nodeSize.width, colDim);
    auto rowSize = getCellSizes(on, nodeSize.height, rowDim);

    int x = 0, y = 0;
    for (auto* c : nodes) {
        c->setContentSize(CCSize{colSize[colidx], rowSize[rowidx]});
        c->setPosition(
            x,typeinfo_cast<Container*>(c)!=nullptr?y:nodeSize.height-y
        );
        x+=colSize[colidx];
        colidx++;
        if (colidx==numcols) {
            y+=rowSize[rowidx];
            colidx = 0; rowidx++;
            x = 0;
            if (rowidx==numrows)break;
        }  
    }
};

bool GridContainer::init(
    std::vector<Dimension> columnDims, std::vector<Dimension> rowDims
) {
    if (!Container::init()) return false;
    setLayout(GridLayout::create(columnDims, rowDims));
    addListener("nodeLayoutUpdate",[this](NodeEvent* ee){
        auto e = static_cast<NodeLayoutUpdate*>(ee);
        if (
            e->type == NodeLayoutUpdateType::Size 
            || 
            e->type == NodeLayoutUpdateType::All
        ) updateLayout();
    });
    return true;
};