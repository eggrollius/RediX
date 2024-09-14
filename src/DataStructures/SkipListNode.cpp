#ifndef SKIP_LIST_NODE
#define SKIP_LIST_NODE

#include <vector>

template <typename KeyType, typename ScoreType>
class SkipListNode {
private:
    KeyType key;
    ScoreType score;

public:
    std::vector<SkipListNode*> next;

    // Constructor that initializes both key and score with a given height
    SkipListNode(const KeyType& key, const ScoreType& score, int height)
        : key(key), score(score), next(height + 1, nullptr) {}

    // Constructor that only initializes the height (used for the sentinel node)
    SkipListNode(int height)
        : next(height + 1, nullptr) {}

    // Returns the height of this node (number of levels)
    int height() const {
        return next.size() - 1;
    }

    // Accessor for the key
    KeyType get_key() const {
        return this->key;
    }

    // Accessor for the score
    ScoreType get_score() const {
        return this->score;
    }
};

#endif // SKIP_LIST_NODE
