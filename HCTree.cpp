/*
 * Name: Ojeen Gammah
 * Email: ogammah@ucsd.edu
 *
 */
#include "HCTree.hpp"

const int HCTree::uniquebyte = 256;
const int HCTree::buffer = 16;
const int HCTree::byte_size = 8;

void HCTree::destructHelper(HCNode* current) {
    if (current->c1){
        destructHelper(current->c1);
    }
    if (current->c0){
        destructHelper(current->c0);
    }
    delete current;
}

HCTree::~HCTree() {
    if (root) {
        destructHelper(root);
    }
}

void HCTree::build(const vector<int>&freqs) {
    std::priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;
    for (int i =0; i<uniquebyte; i++){
        if (freqs[i] > 0){
            leaves[i] = new HCNode(freqs[i], static_cast <unsigned char> (i));
            pq.push(leaves[i]);
        }
        else {
            leaves[i] = nullptr;
        }
    }
    while (pq.size() > 1){
        HCNode* hcn1 = pq.top();
        pq.pop();
        HCNode* hcn2 = pq.top();
        pq.pop();
        HCNode* temp = new HCNode(hcn1->count + hcn2->count, 0);
        hcn1->p = temp;
        hcn2->p = temp;
        if (*hcn2 < *hcn1) {
            temp->c0 = hcn1;
            temp->c1 = hcn2;
        }
        else {
            temp->c0 = hcn2;
            temp->c1 = hcn1;
        }
        pq.push(temp);
    }
    root = pq.top();
    pq.pop();
}

void HCTree::encode(unsigned char symbol, FancyOutputStream & out) const{
    HCNode* current = leaves[symbol];
    char bits[buffer];
    int pos = buffer;
    while (current->p){
        pos--;
        if (current == current->p->c0) {
            bits[pos] = 0;
        }
        else {
            bits[pos] = 1;
        }
        current = current->p;
    }
    while (pos < buffer) {
        out.write_bit(bits[pos]);
        pos++;
    }
}

unsigned char HCTree::decode(FancyInputStream & in) const {
    HCNode* current = root;
    while (current->c1){
        if (in.read_bit() == 1){
            current = current->c1;
        }
        else {
            current = current->c0;
        }
    }
    return current->symbol;
}

unsigned long HCTree::getFileSize() const {
    return filesize;
}
void HCTree::writeHeader(FancyOutputStream & out, unsigned long size) {
    out.write<unsigned long>(size);
    filesize = size;
    wrHdHelper(out, root);
}

void HCTree::wrHdHelper(FancyOutputStream & out, HCNode* current) const {
    if (current->c0) {
        out.write_bit(0);
        wrHdHelper(out, current->c0);
        wrHdHelper(out, current->c1);
    }
    else {
        out.write_bit(1);

        for (int i = 0; i < byte_size; i++) {
            out.write_bit((current->symbol >> (byte_size-1 - i)) & 1);
        }
    }
}

void HCTree::rdHdHelper(FancyInputStream & in, HCNode* current) {
    if (!in.good()) return;
    unsigned char bit = in.read_bit();
    if (bit == 0) {
        current->c0 = new HCNode(0,0);
        rdHdHelper(in, current->c0);
        
    }
    else {
        unsigned char t = 0;
        for (int i = 0; i < byte_size; i++){
            bit = in.read_bit();
            t |= (bit << (byte_size-1 - i));
        }
        current->c0 = new HCNode(1, t);
    }

    bit = in.read_bit();
    if (bit == 0) {
        current->c1 = new HCNode(0,0);
        rdHdHelper(in, current->c1);
    }
    else {
        unsigned char t = 0;
        for (int i = 0; i < byte_size; i++){
            bit = in.read_bit();
            t |= (bit << (byte_size-1 - i));
        }
        current->c1 = new HCNode(1, t);
    }
}

void HCTree::readHeader(FancyInputStream & in) {
    filesize = in.read<unsigned long>();
    in.read_bit();
    root = new HCNode(0,0);
    rdHdHelper(in, root);
}
