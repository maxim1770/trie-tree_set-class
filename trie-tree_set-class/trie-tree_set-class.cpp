#include <iostream>
#include <cctype>

#define ALPHABET_SIZE 33
#define ROOT_LETTER '\0'//корень дерева

using namespace std;

struct ValueContainer//показывает на конец слова
{
private:
    int value;

public:
    ValueContainer(int value)//конструктор
    {
        this->value = value;
    }

    int getValue()//возвращает знач
    {
        return this->value;
    }

};

struct TrieNode
{
private:
    char letter;
    ValueContainer* value;//конец или нет
    TrieNode* parent;//указ на род
    TrieNode** children;//указ на дет

    int findChildIndex(char letter)
    {
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            TrieNode* node = this->children[i];
            if (node != nullptr && node->letter == letter)
                return i;
        }

        return -1;
    }

public:
    TrieNode(char letter, ValueContainer* value = nullptr)//конструктор
    {
        this->letter = letter;
        this->value = value;
        this->parent = nullptr;
        this->children = new TrieNode * [ALPHABET_SIZE];

        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            this->children[i] = nullptr;
        }
    }


    TrieNode* findChild(char letter)//возращ указ на яч
    {
        int index = this->findChildIndex(letter);

        return index > -1 ? this->children[index] : nullptr;
    }

    TrieNode* addChild(char letter, ValueContainer* value = nullptr)//добав ребенка
    {
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            if (!this->children[i])
            {
                TrieNode* node = new TrieNode(letter, value);
                node->parent = this;

                this->children[i] = node;

                return node;
            }
        }
        return nullptr;
    }

    void deleteChild(char letter)//удаление реб
    {
        int index = this->findChildIndex(letter);
        if (index == -1)
            return;


        delete this->children[index];
        this->children[index] = nullptr;

    }

    void deleteSelf()//удаление текущ яч
    {
        if (this->parent)
            this->parent->deleteChild(this->letter);

    }

    bool hasChildren()//есть ли дети??
    {
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            TrieNode* node = this->children[i];
            if (node != nullptr)
                return true;
        }

        return false;
    }

    void setValue(ValueContainer* value)//устанавливает Value 
    {
        this->value = value;
    }

    ValueContainer* getValue()//получаем Value 
    {
        return this->value;
    }

    TrieNode* getParent()//получаем родителя 
    {
        return this->parent;
    }

    char getLetter()//получаем букву 
    {
        return this->letter;
    }

    void print(TrieNode* node, string str)//проход в глубину по дереву 
    {
        TrieNode* current;
        current = node;
        if (node != nullptr)
        {

            for (int i = 0; i < ALPHABET_SIZE; i++)
            {
                if (current->children[i] != nullptr)
                {
                    str = str + current->children[i]->letter;
                    print(current->children[i], str);
                    str.pop_back();
                }
            }
            if (current->getValue() != nullptr)
                cout << "\n" << str;
        }

    }

    void count(TrieNode* node, int* c)//считает колво слов 
    {
        TrieNode* current;
        current = node;
        if (node != nullptr)
        {

            for (int i = 0; i < ALPHABET_SIZE; i++)
            {
                if (current->children[i] != nullptr)
                {
                    count(current->children[i], c);
                }
            }
            if (current->getValue() != nullptr)
                *c = *c + 1;
        }
    }

    void allWords(TrieNode* node, int n, string* str, string str1)//функ кот находит слова и записывает в массив строк 
    {
        TrieNode* current;
        current = node;
        if (node != nullptr)
        {
            for (int i = 0; i < ALPHABET_SIZE; i++)
            {
                if (current->children[i] != nullptr)
                {
                    str1 = str1 + current->children[i]->letter;
                    allWords(current->children[i], n, str, str1);
                    str1.pop_back();
                }
            }
            if (current->getValue() != nullptr)
            {
                int i = 0;
                while (str[i] != "0")
                    i++;
                str[i] = str1;
            }

        }
    }
};

struct TrieTree
{
private:
    TrieNode* root;//корень 


    TrieNode* getNode(string key)
    {
        TrieNode* node = this->root;

        for (int i = 0; i < key.size(); i++)
        {
            char letter = key[i];
            node = node->findChild(letter);
            if (!node)
                return nullptr;
        }

        return node;
    }

public:
    TrieTree()//конструктор
    {
        this->root = new TrieNode(ROOT_LETTER);
    }

    ValueContainer* get(string key)
    {
        TrieNode* node = this->getNode(key);

        return node ? node->getValue() : nullptr;
    }

    bool set(string key, ValueContainer* value)
    {
        TrieNode* node = this->root;

        bool isNewBranch = false;

        for (int i = 0; i < key.size(); i++)
        {
            char letter = key[i];
            TrieNode* next = isNewBranch ? nullptr : node->findChild(letter);
            if (next)
                node = next;
            else
            {
                node = node->addChild(letter);
                isNewBranch = true;
            }
        }

        if (node)
            node->setValue(value);

        return node != nullptr;
    }

    bool remove(string key)
    {
        TrieNode* node = this->getNode(key);sf
        if (!node)
            return false;

        TrieNode* current = node;

        while (current && current->getLetter() != ROOT_LETTER)
        {
            TrieNode* parent = current->getParent();

            if (current->hasChildren())
            {
                if (current == node)
                    current->setValue(nullptr);
                return true;
            }

            current->deleteSelf();
            current = parent;
        }

        return true;
    }
    void allWords(int n, string* str)//возвращ все слова из дерева
    {
        string str1;
        root->allWords(root, n, str, str1);
    }
    void count(int* c)//колво слов
    {
        root->count(root, c);
    }
    void print()
    {
        string str;
        root->print(root, str);
    }
    // void intersection(string* str2, string* str, int n)//ищет слова которые есть и в первом и во втором дереве и возвращ их
    // {
     //    for (int i = 0; i < n; i++)
    //     {
     //        if (this->get(str[i]))
    //         {
     //            int j = 0;
     //            while (str2[j] != "0")
     //                j++;
      //           str2[j] = str[i];
       //      }
     //
       //  }
  //   }



    void intersection(TrieTree tree)
    {

        int* c = new int[1];
        *c = 0;
        tree.count(c);
        string* str = new string[*c];
        for (int i = 0; i < *c; i++)
        {
            str[i] = "0";
        }
        tree.allWords(*c, str);
        for (int i = 0; i < *c; i++)
        {
            if (this->get(str[i]))
            {
                cout << str[i] << "\n";
            }

        }
    }

};

int main()
{
    int* c = new int[1];
    *c = 0;
    TrieTree tree, tree2;
    int i = 1;
    string word;


    cout << "Input word:";
    cin >> word;
    while (word != "0")
    {
        ValueContainer value(i);//порядковый номер слова
        tree.set(word, &value);
        cin >> word;
        i++;
    }

    cout << "Input word:";
    cin >> word;
    while (word != "0")
    {
        ValueContainer value(i);
        tree2.set(word, &value);
        cin >> word;
        i++;
    }

    cout << "\n" << "1:";
    tree.count(c);
    string* str = new string[*c];
    string* str2 = new string[*c];
    for (int i = 0; i < *c; i++)//заполняем массив сторк 0
    {
        str[i] = "0";
        str2[i] = "0";
    }
    cout << "\n 1 tree:\n";
    tree.print();
    cout << "\n 2 tree:\n";
    tree2.print();


    cout << "\nIntersection:\n";
    tree2.intersection(tree);

    //tree.allWords(*c, str);//запишем все слова из 1 дер в массив str
    //tree2.intersection(str2, str, *c);
    //cout << "\nIntersection:\n";
    i = 0;
    while (str2[i] != "0")
    {
        cout << str2[i] << "\n";
        i++;
    }

}