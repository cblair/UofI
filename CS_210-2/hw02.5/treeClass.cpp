#include "treeClass.h"

double Node::scaleX_ = 1.0;
double Node::scaleY_ = 1.0;
double Node::rotate_ = 0;
double Node::rotateLabel_ = 30;
double Node::rotateLeaf_ = 0;
double Node::translateX_ = 72.0;  // 1 inch margin
double Node::translateY_ = 0.0;
double Node::yDelta_ = 20.0;      // lateral distance between species
double Node::minLength_ = 5.0;    // minium vertical distance for species
int Node::lineWidth_ = 2;         // weight of line
char *Node::fontName_ = "Helvetica"; // "Times-Roman";
int Node::fontSize_ = 12;


// these should be class variables.  I'll do that someday.
static int llx=0;
static int lly=0;
static int urx=300;
static int ury=300;
static double minx, miny, maxx, maxy;

// // // // // // // // // // // // // // // // // // // //
//
// Ultra-Basic Postscript Drawing Routines
//

// *estimate* size of drawing
void addExtent(double x, double y)
{
    if (x<minx) minx = x;
    if (y<miny) miny = y;
    if (x>maxx) maxx = x;
    if (y>maxy) maxy = y;
}

// call before starting a drawing
void drawStart(double x, double y, 
               double angle, 
               double sx, double sy,
               char* fontName, 
               int fontSize,
               int lineWidth)
{
    printf("%%!PS-Adobe-2.0\n");
    printf("%%!BoundingBox: %d %d %d %d\n", llx, lly, urx, ury);
    printf("%% Tree drawing program by Robert Heckendorn\n");
    printf("1 setlinecap 1 setlinejoin\n");
    printf("/%s findfont %d scalefont setfont\n", fontName, fontSize);

    printf("%5.1lf %5.1lf translate %5.1lf rotate\n", x, y, angle);
    printf("%5.1lf %5.1lf scale\n", sx, sy);
    printf("newpath\n");
    printf("%d setlinewidth\n", lineWidth);
}

// draw a string
void drawStr(double x, double y, double angle, char*name)
{
    if (*name) {
        addExtent(x, y);
        printf("%5.1lf %5.1lf moveto\n", x, y);
        printf("gsave %5.1lf rotate\n", angle);
        printf("( %s) show grestore\n", name);
    }

    return;
}


// draw a centered string
void drawStrCentered(double x, double y, double angle, char*name)
{
    if (*name) {
        addExtent(x, y);
        printf("%5.1lf %5.1lf moveto\n", x, y);
        printf("gsave %5.1lf rotate\n", angle);
        printf("(%s) dup stringwidth pop 2 div neg 0 rmoveto show grestore\n", name);
    }

    return;
}


// draw a line
void drawLine(double x1, double y1, double x2, double y2, char *name, int fontSize)
{
    addExtent(x1, y1);
    addExtent(x2, y2);
    printf("%5.1lf %5.1lf moveto\n%5.1lf %5.1lf lineto\n",
           x1, y1, x2, y2);
    drawStrCentered((x1+x2)/2.0, (y1+y2)/2.0+.2*fontSize, 0.0, name);

    return;
}



// end drawing
void drawEnd()
{
    printf("stroke\n");
    printf("showpage\n");
    printf("%% (%lf, %lf)  (%lf, %lf)\n", minx, miny, maxx, maxy);

    return;
}



// // // // // // // // // // // // // // // // // // // //
//
// CLASS: Node
//

// // // // // // // // // // // // // // // // 
//
// Constructors
//

Node::Node()
{
    parent_ = 0;
    child_ = 0;
    sibling_ = 0;
    len_ =  minLength_;
    nodeName_ = "";
    edgeName_ = "";
}

Node::Node(Node *parent)
{
    parent_ = parent;
    child_ = 0;
    sibling_ = 0;
    len_ =  minLength_;
    nodeName_ = "";
    edgeName_ = "";
}



Node *Node::copy()
{
    return copy(NULL);
}


Node *Node::copy(Node *newParent)
{
    Node *newguy;

    newguy = new Node;

    newguy->parent_ = newParent;
    if (child_) newguy->child_ = child_->copy(newguy);
    else newguy->child_ = NULL;
    if (sibling_) newguy->sibling_ =  sibling_->copy(newParent);
    else newguy->sibling_ = NULL;

    newguy->len_ =  len_;
    newguy->nodeName_ = strdup(nodeName_);
    newguy->edgeName_ = strdup(edgeName_);

    return newguy;
}    




// // // // // // // // // // // // // // // // 
//
// Drawing a Tree
//

// Drawing support for a node
//
// from parent the depth distance x of parent and y of neighbor
// return rightmost extension y of self
//
// A node is responsible for drawing a cross bar if it is not
// a leaf and all nodes N_i draw the bar of length N_i.len_

// the private draw routine
void Node::draw(double x, double &y, double &newcenter)
{
    Node *c;
    double nextx, nexty, firstcenter, lastcenter;

    c = child_;
    nextx = x + len_;
    nexty = y;

    // if has any children
    if (c) {
        // draw first child
        c->draw(nextx, nexty, firstcenter);
        lastcenter = firstcenter;
        
        // draw remaining children
	c = c->sibling_;
	while (c) {
	    c->draw(nextx, nexty, lastcenter);
	    c = c->sibling_;
        }
        // nexty = rightmost of children
        // firstcenter to lastcenter connects the bounds of children's bars

        // draw crossbar
	drawLine(nextx, firstcenter, nextx, lastcenter, "", fontSize_);
	newcenter = (firstcenter + lastcenter)/2.0;

        // draw edge down to crossbar
	drawLine(nextx, newcenter, x, newcenter, edgeName_, fontSize_);

        // label internal node
	drawStr(nextx, newcenter-.3*fontSize_, rotateLabel_, nodeName_);

	y = nexty;
    }
    // else is a leaf
    else {
        newcenter = nexty + yDelta_;  // add in for width of a leaf
//        newcenter = nexty; 
	y = newcenter;

        // draw edge down to crossbar
	drawLine(nextx, newcenter, x, newcenter, edgeName_, fontSize_);

        // label node
	drawStr(nextx, newcenter-.3*fontSize_, rotateLeaf_, nodeName_);
    }
}

// draw the tree with this root node
void Node::draw()
{
    double x, y, center;
    x = llx;
    y = lly;
    drawStart(translateX_, translateY_, rotate_, scaleX_, scaleY_, fontName_, fontSize_, lineWidth_);
    draw(x, y, center);
    drawEnd();
}


// private routine for displaying internal form of tree
void Node::displayAux()
{
    if (this) {
        printf("Node: 0x%08x", (unsigned int)this);
	printf("  name: \"%s\"", nodeName_);
//RH	printf("  edge: \"%s\"", edgeName_);
	printf("  len: %4.1lf", len_ );
        printf("  parent: 0x%08x", (unsigned int)parent_);
        printf("  child: 0x%08x", (unsigned int)child_);
        printf("  sibling: 0x%08x", (unsigned int)sibling_);
        printf("\n");
        fflush(stdout);
    }
    else {
        printf("Attempting to display a node at location 0\n");
    }
}



// display internal form of the tree
void Node::display(int level)
{
    Node *ptr;

    for (int i=0; i<level; i++) printf("   ");
    displayAux();

    ptr = child_;
    while (ptr) {
        ptr->display(level+1);
        ptr = ptr->sibling_;
    }
    fflush(stdout);
}



// add a child node beneath a node  
// this parents all the siblings of this child as well
void Node::setChild(Node *child) 
{ 
    if (child==NULL) fprintf(stderr, "WARNING(setChild): trying to add an NULL child\n");
    child_ = child;
    while (child) {
        child->parent_ = this;
        child = child->sibling_;
    }
}



// add a sibling to this child node
void Node::addSibling(Node *sib)
{
    Node *node;

    if (sib==NULL) fprintf(stderr, "WARNING(addSibling): trying to add an NULL sibling\n");
    node = this;
    while (node->sibling_) node = node->sibling_;
    node->sibling_ = sib;
}


