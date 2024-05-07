#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) //Lista
{
    TreeMap *new = (TreeMap *)malloc(sizeof(TreeMap));
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) //Lista
{
    if (tree == NULL || key == NULL || value == NULL) return;

    if (tree->root == NULL) tree->root = createTreeNode(key, value);
    else
    {
        TreeNode *aux = tree->root;
        
        while (aux != NULL)
        {
            if (is_equal(tree, key, aux->pair->key)) return;
            if (tree->lower_than(key, aux->pair->key) == 1)
            { //Verificamos si es menor
                if (aux->left == NULL)
                {
                    TreeNode *new = createTreeNode(key, value);
                    new->parent = aux;
                    aux->left = new;
                    tree->current = new;
                    break;
                }
                else aux = aux->left;
            } 
            else
            {
                if (aux->right == NULL)
                { //En otro caso, será mayor
                    TreeNode *new = createTreeNode(key, value);
                    new->parent = aux;
                    aux->right = new;
                    tree->current = new;
                    break;
                }
                else aux = aux->right;
            }

        }
    }
    
    return;
}

TreeNode * minimum(TreeNode *x) //Lista
{
    while (x->left != NULL)
    {
        x = x->left;
    }
    return x;
}

TreeNode * maximum(TreeNode *x) //Lista
{
    while (x->right != NULL)
    {
        x = x->right;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) //Lista
{
    if (tree == NULL || node == NULL) return;
    if (node->left == NULL && node->right == NULL)
    { //Si no tiene hijos
        if (node->parent->left == node) node->parent->left = NULL;
        else node->parent->right = NULL;
    }
    else
    {
        //En caso de un solo hijo
        if (node->left != NULL && node->right == NULL)
        {
            if (node->parent->left == node) 
            {
                node->parent->left = node->left;
                node->left->parent = node->parent;
            }

            else 
            {
                node->parent->right = node->left;
                node->left->parent = node->parent;
            }
        }
        if (node->right != NULL && node->left == NULL)
        {
            if (node->parent->left == node) 
            {
                node->parent->left = node->right;
                node->right->parent = node->parent;
            }

            else 
            {
                node->parent->right = node->right;
                node->right->parent = node->parent;
            }
        }
        //En caso de tener 2 hijos
        if (node->left != NULL && node->right != NULL)
        {
            TreeNode *min = minimum(node->right);
            node->pair->key = min->pair->key;
            node->pair->value = min->pair->value;
            removeNode(tree, min);
        }
            
    }
    return;
}

   


void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}



 
Pair * searchTreeMap(TreeMap * tree, void* key) //Lista
{
    TreeNode *aux = tree->root;
    while (aux != NULL)
        {
            if (is_equal(tree, key, aux->pair->key) == 1)
            { //Retornar en caso de que sean iguales
                tree->current = aux;
                return aux->pair;
            }
            else
            {
                if (tree->lower_than(key, aux->pair->key) == 1)
                { //Si es menor, vamos a la izquierda
                    aux = aux->left;
                }
                else
                { //Si es mayor, vamos a la derecha
                    aux = aux->right;
                }
            }
        }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) 
{
    Pair *dato = searchTreeMap(tree, key);
    if (dato != NULL) return dato;
    
    TreeNode *aux = tree->root;
    while (aux != NULL)
    {
        if (is_equal(tree, key, aux->pair->key) == 1)
        {
            return aux->pair;
        }
            
        else 
        {
            if (tree->lower_than(key, aux->pair->key) == 1)
            {
                if (aux->left == NULL) break;
                aux = aux->left;
            }
            else
            {
                if (aux->right == NULL) break;
                aux = aux->right;
            }
        }
    }
    return aux->pair;
    
}

Pair * firstTreeMap(TreeMap * tree) //Lista
{
    TreeNode *aux = tree->root;
    if (aux == NULL) return NULL;
    while (aux->left != NULL)
        {
            aux = aux->left;
        }
    tree->current = aux;
    return aux->pair;
}

Pair * nextTreeMap(TreeMap * tree) //Lista
{
    TreeNode *aux = tree->current;
    if (aux == NULL) return NULL;
    if (aux->right != NULL)
    {
        aux = aux->right;
        while (aux->left != NULL)
            {
                aux = aux->left;
            }
        tree->current = aux;
        return aux->pair;
    }
    else
    {
        while (aux->parent != NULL && aux->parent->right == aux)
            {
                aux = aux->parent;
            }
        tree->current = aux->parent;
        if (aux->parent == NULL) return NULL;
        else return aux->parent->pair;
    }
    
    
    return NULL;  
}
