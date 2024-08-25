#include "GridContainer.hpp"

float calcSize(float size, float s, Dimension dim) {
    if (dim.mode == GridSizeMode::Relative) return s*size;
    return s;
} 


std::vector<float> getCellSizes(CCNode* on, float size, std::vector<Dimension>& dimensions) {
    int idx = 0;
    std::vector<float> SizeF;
    std::map<Dimension,float,std::equal_to<Dimension>> Size;
    std::vector<Dimension> Queue;
    std::vector<Dimension> AutoSizeQueue;
    int numItems = dimensions.size();
    // essentially copying
    float remainingSize = 0+size;

    for (;idx<numItems;idx++) {
        auto dim = dimensions[idx];
        if (dim.mode == GridSizeMode::Absolute || dim.mode == GridSizeMode::Relative) {
            auto s = calcSize(size,std::max((float)dim.size, (float)dim.minSize),dim);
            // might be better to just let it take the entire space, 
            // but i didnt support multicell allocation yet,
            // nor did the osu!framework implementation

            // nvm i think they did autosize mode exists

            // nvm you dont have a copy of a coldim every rowdims
            if (dim.maxSize != -1) s = std::min(s, (float)dim.maxSize);
            Size[dim] = s;
            remainingSize -= s;
        } else {
            if (dim.mode == GridSizeMode::AutoSize) AutoSizeQueue.push_back(dim);
            else Queue.push_back(dim);
        }
    }
    auto qcount = Queue.size();
    auto aqcount = AutoSizeQueue.size();
    for (auto dim : Queue) {
        auto divSize = remainingSize/qcount;
        float s;
        if (aqcount == 0) {
            s = calcSize(size, std::max(divSize, (float)dim.minSize), dim);
            if (dim.maxSize != -1) s = std::min(s, (float)dim.maxSize);
        } 
        // give em a slot
        else {
            s = calcSize(size, dim.minSize, dim);
        }
        Size[dim] = s;
        remainingSize -= s;
        qcount--;
    }
    if (aqcount != 0) {    
        float maxSize = remainingSize / aqcount;
        for (auto dim : AutoSizeQueue) {Size[dim] = maxSize;}
    }
    
    idx = 0;
    for (;idx<numItems;idx++) {
        SizeF.push_back(Size[dimensions[idx]]);
    };
    return SizeF;

}

void GridLayout::apply(CCNode* on) {
    log::debug("[GridLayout]: meow");
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
        c->setAnchorPoint(ccp(0,1));
        if (auto cc = typeinfo_cast<Container*>(c)) {
            cc->setAnchor(Anchor::TopLeft);
            cc->setSizeUnit(Unit::OpenGL, Unit::OpenGL);
            cc->setPositionUnit(Unit::OpenGL, Unit::OpenGL);
        }
        colidx++;
        if (colidx==numcols-1) {colidx = 0; rowidx++;}
    }

    colidx = 0; rowidx = 0;
    
    auto colSize = getCellSizes(on, nodeSize.width, colDim);
    auto rowSize = getCellSizes(on, nodeSize.height, rowDim);

    log::debug("[GridLayout]: \nColumn sizes: {}\nRow sizes: {}", colSize,rowSize);

    int x = 0, y = 0;
    for (auto* c : nodes) {
        c->setContentSize(CCSize{colSize[colidx], rowSize[rowidx]});
        c->setPosition(x,y);
        x+=colSize[colidx];
        colidx++;
        if (colidx==numcols-1) {
            y+=rowSize[rowidx];
            colidx = 0; rowidx++;
            x = 0;
        }  
    }
};

bool GridContainer::init(std::vector<Dimension> columnDimensions, std::vector<Dimension> rowDimensions) {
    if (!Container::init()) return false;
    setLayout(GridLayout::create(columnDimensions, rowDimensions));
    addListener("nodeLayoutUpdate",[this](NodeEvent* ee){
        auto e = static_cast<NodeLayoutUpdate*>(ee);
        if (e->type == NodeLayoutUpdateType::Size || e->type == NodeLayoutUpdateType::All) updateLayout();
    });
    return true;
};