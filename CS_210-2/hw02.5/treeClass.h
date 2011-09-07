#ifndef TREECLASSH
#define TREECLASSH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// // // // // // // // // // // // // // // // // // // //
//
// CLASS: Node
//

class Node {
// class variables
private:
    static double scaleX_;
    static double scaleY_;
    static double rotate_;
    static double rotateLabel_;
    static double rotateLeaf_;
    static double translateX_;
    static double translateY_;
    static double yDelta_;       // use as spacing
    static double minLength_;    // use as default length
    static int lineWidth_;
    static char *fontName_;
    static int fontSize_;

// data
private:
    Node *parent_;       // my parent  (NULL if root)
    Node *child_;        // the first child (other children are siblings)
    Node *sibling_;      // my siblings (this allows many children)
    double len_;         // length of arc down to my node
    char *nodeName_;     // label at node
    char *edgeName_;     // label along edge

// methods
private:
    void displayAux();
    void draw(double x, double &y, double &newcenter);
    Node *copy(Node *newParent);

// constructors
public:
    Node();
    Node(Node *parent);

// accessors
public:
    void setChild(Node *child);                     // create a child
    void addSibling(Node *sib);                     // create a sibling
    Node *copy();                                   // copy a tree
    void setName(char *name) { nodeName_ = name; }  // name the node
    void setEName(char *name) { edgeName_ = name; } // name the edge leading to the node
    void setLen(double len) {len_ = len; }               // set length of edge
    void setRotate(double angle) {rotate_ = angle; }  // rotation for picture
    void setRotateLabel(double angle) {rotateLabel_ = angle; }  // rotation for internal node label
    void setRotateLeaf(double angle) {rotateLeaf_ = angle; }  // rotation for leaf label
    void setTranslate(double x, double y) { translateX_ = x; translateY_ = y; }
    void setScale(double x, double y) { scaleX_ = x; scaleY_ = y; }
    void setYDelta(double delta) { yDelta_ = delta; } // spacing between species
    void setMinLen(double x) { minLength_ = x; }                // default lenth of a edge
    void setLineWidth(int w) { lineWidth_ = w; }                // width of lines
    void setFontName(char *name) { fontName_ = strdup(name); }  // type font of labels
    void setFontSize(int size) {fontSize_ = size; }             // font size in points

// misc
    void display(int level=0);                        // show internal form of tree for debugging
    void draw();                                      // draw the tree
};

#endif
