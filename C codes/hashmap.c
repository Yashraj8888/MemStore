#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100

// Node representing a key-value pair
typedef struct Node {
    char *key;
    char *value;
    struct Node *next;
} Node;

// Hash table structure
typedef struct HashTable {
    Node *buckets[TABLE_SIZE];
} HashTable;

// djb2 hash function
unsigned long hash_function(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % TABLE_SIZE;
}

// Create a new node
typedef struct Node Node;
Node *create_node(const char *key, const char *value) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->key = strdup(key);
    newNode->value = strdup(value);
    newNode->next = NULL;
    return newNode;
}

// Insert or update key-value pair
void set(HashTable *table, const char *key, const char *value) {
    unsigned long index = hash_function(key);
    Node *head = table->buckets[index];

    Node *temp = head;
    while (temp) {
        if (strcmp(temp->key, key) == 0) {
            free(temp->value);
            temp->value = strdup(value);
            return;
        }
        temp = temp->next;
    }

    Node *newNode = create_node(key, value);
    newNode->next = head;
    table->buckets[index] = newNode;
}

// Retrieve value by key
char *get(HashTable *table, const char *key) {
    unsigned long index = hash_function(key);
    Node *temp = table->buckets[index];

    while (temp) {
        if (strcmp(temp->key, key) == 0) {
            return temp->value;
        }
        temp = temp->next;
    }

    return NULL;
}

// Delete a key-value pair
void delete_key(HashTable *table, const char *key) {
    unsigned long index = hash_function(key);
    Node *temp = table->buckets[index];
    Node *prev = NULL;

    while (temp) {
        if (strcmp(temp->key, key) == 0) {
            if (prev)
                prev->next = temp->next;
            else
                table->buckets[index] = temp->next;

            free(temp->key);
            free(temp->value);
            free(temp);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
}

// Free entire table
void free_table(HashTable *table) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node *temp = table->buckets[i];
        while (temp) {
            Node *next = temp->next;
            free(temp->key);
            free(temp->value);
            free(temp);
            temp = next;
        }
    }
}

// Simple test main function
int main() {
    HashTable table = {0};

    set(&table, "name", "Yashraj");
    set(&table, "role", "SDE1");
    set(&table, "lang", "C");

    printf("name = %s\n", get(&table, "name"));
    printf("role = %s\n", get(&table, "role"));
    printf("lang = %s\n", get(&table, "lang"));

    delete_key(&table, "role");
    printf("role after delete = %s\n", get(&table, "role"));

    free_table(&table);
    return 0;
}
