/*
 * Name: Ojeen Gammah
 * Email: ogammah@ucsd.edu
 *
 * This file provides a skeleton for a huffman tree. For students, feel free
 * to implement the tree in HCTree.cpp (you can create a new file).
 */

#ifndef HCTREE_HPP
#define HCTREE_HPP
#include <queue>
#include <vector>
#include <fstream>
#include "Helper.hpp"
using namespace std;

/**
 * A Huffman Code Tree class
 */
class HCTree {
private:
    unsigned long filesize;
    HCNode* root;
    vector<HCNode*> leaves;
    static const int uniquebyte;
    static const int buffer;
    static const int byte_size;

public:
    /**
     * Constructor, which initializes everything to null pointers
     */
    HCTree() : root(nullptr) {
        leaves = vector<HCNode*>(256, nullptr);
    }

    ~HCTree();

    /**
     * Use the Huffman algorithm to build a Huffman coding tree.
     * PRECONDITION:  freqs is a vector of ints, such that freqs[i] is the
     *                frequency of occurrence of byte i in the input file.
     * POSTCONDITION: root points to the root of the trie, and leaves[i]
     *                points to the leaf node containing byte i.
     *
     * @param freqs frequency vector
     */
    void build(const vector<int>& freqs);

    /**
     * Write to the given FancyOutputStream the sequence of bits coding the
     * given symbol.
     * PRECONDITION: build() has been called, to create the coding tree,
     *               and initialize root pointer and leaves vector.
     *
     * @param symbol symbol to encode
     * @param out output stream for the encoded bits
     */
    void encode(unsigned char symbol, FancyOutputStream & out) const;

    /**
     * Return symbol coded in the next sequence of bits from the stream.
     * PRECONDITION: build() has been called, to create the coding tree, and
     *               initialize root pointer and leaves vector.
     *               OTHERWISE, readHeader() has been called for the same reason.
     *
     * @param in input stream to find encoded bits
     * @return a single char decoded from the input stream
     */
    unsigned char decode(FancyInputStream & in) const;

    /**
     * Public helper function to write a header to the encoded file.
     * PRECONDITION: build() has been called, to create the coding tree, and
     *               initialize root pointer and leaves vector.
     *
     * @param out FancyOutputStream with file open for writing.
     */
    void writeHeader(FancyOutputStream & out, unsigned long size);

    /**
     * Public helper function to read a header from an encoded file,
     * and create the encoding tree from that information.
     *
     * @param in FancyInputStream with file open for reading.
     */
    void readHeader(FancyInputStream & in);

    unsigned long getFileSize() const;
    
private:
    /**
     * Private helper function to recursively deallocate the HCNodes
     * present in the HCTree. Uses post-order iteration.
     *
     * @param current node to delete after deleting children
     */
    void destructHelper(HCNode* current);

    /**
     * Private helper function to recursively write the tree
     * to the header. Uses pre-order traversal.
     * PRECONDITION: build() has been called, to create the coding tree,
     * initialize root pointer and leaves vector.
     *
     * @param out FancyOutputStream with file to write header to
     * @param current the current node in the pre-order traversal
     */
    void wrHdHelper(FancyOutputStream & out, HCNode* current) const;

    /**
     *
     *
     */
    void rdHdHelper(FancyInputStream & in, HCNode* current);

};
#endif // HCTREE_HPP
