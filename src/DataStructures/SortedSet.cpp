#ifndef SORTED_SET
#define SORTED_SET

#include <unordered_map>
#include <vector>
#include "SkipListNode.cpp"
#include <cstdlib>
#include <ctime>

template <typename KeyType, typename ScoreType>
class SortedSet {
private:
    SkipListNode<KeyType, ScoreType>* sentinel;
    int current_list_height = 0;
    int cardinality = 0;

    std::unordered_map<KeyType, ScoreType> lookup; // Use a map for efficient key lookup

    int pick_height() {
        int result = 0;
        while (std::rand() % 2 == 0) {
            result += 1;
        }
        return result;
    }

public:
    SortedSet() {
        std::srand(std::time(0));
        this->sentinel = new SkipListNode<KeyType, ScoreType>(pick_height());
    }

    ~SortedSet() {
        SkipListNode<KeyType, ScoreType>* current = sentinel;
        while (current != nullptr) {
            SkipListNode<KeyType, ScoreType>* next = current->next[0];
            delete current;
            current = next;
        }
    }

    int size() const {
        return this->cardinality;
    }

    int add(const KeyType& key, const ScoreType& score) {
        if (this->lookup.find(key) != this->lookup.end()) {
            return 0; // Key already exists
        }

        // Add to lookup map
        this->lookup[key] = score;

        // Create a new node with a random height
        int node_height = pick_height();
        SkipListNode<KeyType, ScoreType>* newNode = new SkipListNode<KeyType, ScoreType>(key, score, node_height);

        // Update the maximum height if necessary
        if (node_height > this->current_list_height) {
            this->current_list_height = node_height;
            for (int h = this->sentinel->height(); h <= node_height; ++h) {
                if (this->sentinel->next.size() <= static_cast<size_t>(h)) {
                    this->sentinel->next.push_back(nullptr);
                }
            }
        }

        SkipListNode<KeyType, ScoreType>* u = this->sentinel;
        std::vector<SkipListNode<KeyType, ScoreType>*> update(this->current_list_height + 1, nullptr);

        for (int h = this->current_list_height; h >= 0; --h) {
            while (u->next[h] != nullptr && u->next[h]->get_score() < score) {
                u = u->next[h];
            }
            update[h] = u;
        }

        for (int h = 0; h <= newNode->height(); ++h) {
            newNode->next[h] = update[h]->next[h];
            update[h]->next[h] = newNode;
        }

        this->cardinality += 1;
        return 1;
    }

    int remove(const KeyType& key) {
        if (this->lookup.find(key) == this->lookup.end()) {
            return 0;
        }

        ScoreType score = this->lookup[key]; // Get the score of the element
        this->lookup.erase(key);

        SkipListNode<KeyType, ScoreType>* u = this->sentinel;
        std::vector<SkipListNode<KeyType, ScoreType>*> update(this->current_list_height + 1, nullptr);

        for (int h = this->current_list_height; h >= 0; --h) {
            while (u->next[h] != nullptr && u->next[h]->get_score() < score) {
                u = u->next[h];
            }
            update[h] = u;
        }

        u = u->next[0];

        if (u != nullptr && u->get_key() == key) {
            for (int h = 0; h <= this->current_list_height; ++h) {
                if (update[h]->next[h] == u) {
                    update[h]->next[h] = u->next[h];
                }
            }
            delete u;
            this->cardinality -= 1;

            while (this->current_list_height > 0 && sentinel->next[this->current_list_height] == nullptr) {
                this->current_list_height -= 1;
            }
          return 1;
        }

        return 0;
    }

    bool contains(const KeyType& key) {
        return this->lookup.find(key) != this->lookup.end();
    }

    std::vector<std::pair<KeyType, ScoreType>> range(const ScoreType& min, const ScoreType& max) const {
        std::vector<std::pair<KeyType, ScoreType>> to_return;
        SkipListNode<KeyType, ScoreType>* u = this->sentinel;

        for (int h = this->current_list_height; h >= 0; --h) {
            while (u->next[h] != nullptr && u->next[h]->get_score() < min) {
                u = u->next[h];
            }
        }

        u = u->next[0];

        while (u != nullptr && u->get_score() <= max) {
            std::pair<KeyType, ScoreType> cur(u->get_key(), u->get_score());
            to_return.push_back(cur);
            u = u->next[0];
        }

        return to_return;
    }
};

#endif // SORTED_SET
