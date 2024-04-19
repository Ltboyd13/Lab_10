#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Trie node structure
struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    int count;
};

// Trie structure
struct Trie {
    struct TrieNode *root;
};

struct TrieNode *newNode() {
    struct TrieNode *pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    pNode->count = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        pNode->children[i] = NULL;
    }
    return pNode;
}

struct Trie *createTrie() {
    struct Trie *pTrie = (struct Trie *)malloc(sizeof(struct Trie));
    pTrie->root = newNode();
    return pTrie;
}

void insert(struct Trie *pTrie, char *word) {
    struct TrieNode *pCrawl = pTrie->root;
    int index;
    for (int level = 0; level < strlen(word); level++) {
        index = word[level] - 'a';
        if (!pCrawl->children[index]) {
            pCrawl->children[index] = newNode();
        }
        pCrawl = pCrawl->children[index];
    }
    pCrawl->count++;
}

int numberOfOccurrences(struct Trie *pTrie, char *word) {
    struct TrieNode *pCrawl = pTrie->root;
    int index;
    for (int level = 0; level < strlen(word); level++) {
        index = word[level] - 'a';
        if (!pCrawl->children[index])
            return 0;
        pCrawl = pCrawl->children[index];
    }
    return pCrawl->count;
}

struct Trie *deallocateTrie(struct Trie *pTrie) {
    // Recursive deallocation function would be implemented here
    free(pTrie);
    return NULL;
}

int readDictionary(char *filename, char ***pInWords) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open file");
        return 0;
    }

    int numWords;
    fscanf(file, "%d", &numWords); // First line is the number of words
    *pInWords = malloc(numWords * sizeof(char*));

    char buffer[100];
    for (int i = 0; i < numWords; i++) {
        fscanf(file, "%s", buffer);
        (*pInWords)[i] = strdup(buffer);
    }

    fclose(file);
    return numWords;
}

int main(void) {
    char *inWords[256];
    int numWords = readDictionary("dictionary.txt", inWords);

    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++) {
        insert(pTrie, inWords[i]);
    }

    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    // Free the allocated words
    for (int i = 0; i < numWords; i++) {
        free(inWords[i]);
    }
    free(inWords);

    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL)
        printf("There is an error in this program\n");
    return 0;
}
