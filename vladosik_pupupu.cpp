#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct TCriminal
{
    struct TCriminal  * m_Next;
    char              * m_Name;
    struct TCriminal ** m_Contacts;
    size_t              m_Cnt;
    size_t              m_Capacity;
} TCRIMINAL;

#endif /* __PROGTEST__ */

TCRIMINAL *createRecord(const char *name, TCRIMINAL *next) {
    TCRIMINAL *tmp = (TCRIMINAL *)malloc(sizeof(TCRIMINAL));
    size_t lengthName = strlen(name);
    tmp->m_Name = (char *)malloc((lengthName + 1) * sizeof(char));
    strncpy(tmp->m_Name, name, lengthName);
    tmp->m_Next = next; // Устанавливаем следующий элемент
    tmp->m_Cnt = 0;
    tmp->m_Capacity = 10;
    tmp->m_Contacts = (TCRIMINAL **)malloc(tmp->m_Capacity * sizeof(TCRIMINAL *));

    return tmp;
}


void addContact(TCRIMINAL *dst, TCRIMINAL *contact) {
    dst->m_Contacts[dst->m_Cnt] = contact;
    if(dst->m_Cnt + 1 == dst->m_Capacity) {
        dst->m_Capacity *= 2;
        dst->m_Contacts = (TCRIMINAL **) realloc(dst->m_Contacts, dst->m_Capacity * sizeof(TCRIMINAL *));
    }
    ++dst->m_Cnt;
}

TCRIMINAL *cloneList(TCRIMINAL *node){
    TCRIMINAL *head = nullptr, * tmp_node = node, * tail = nullptr;
    while(true){
        if(tmp_node == nullptr){
            break;
        }else{
            TCRIMINAL * tmp = (TCRIMINAL*)malloc(sizeof(TCRIMINAL));
            size_t str_length = strlen(tmp_node->m_Name);
            tmp->m_Name = (char*)malloc((str_length + 1) * sizeof(char));
            strncpy(tmp->m_Name, tmp_node->m_Name, str_length);
            tmp->m_Capacity = node->m_Capacity;

            if(tail == nullptr){
                head = tmp;
            }else{
                tail->m_Next = tmp;
            }
            tail = tmp;
            tmp_node = tmp_node->m_Next;
        }
    }
    /*
     * если у контакта из старого листа из которого клонируем и у узла в новый наклонированном листе совпадают имена, то вызываем функцию addContact()*/
    tmp_node = node;
    TCRIMINAL * tmp_new_node = head;
    while(true){
        if(tmp_node == nullptr){
            break;
        }
        tmp_new_node->m_Contacts = (TCRIMINAL**)malloc(tmp_new_node->m_Capacity * sizeof(TCRIMINAL*));
        if(tmp_node->m_Cnt != 0){
            for(int i = 0; i < tmp_node->m_Cnt; ++i){
                TCRIMINAL * iterator_new_list = head;
                while(true){
                    if(strcmp(tmp_node->m_Contacts[i]->m_Name, iterator_new_list->m_Name)){
                        iterator_new_list = iterator_new_list->m_Next;
                    }else{
                        addContact(tmp_new_node, iterator_new_list);
                        break;
                    }
                }
            }
        }
        tmp_node = tmp_node->m_Next;
        tmp_new_node = tmp_new_node->m_Next;
    }
    return head;
}




void freeList(TCRIMINAL *src) {

}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
    TCRIMINAL * a, *b;
    char tmp[100];

    assert ( sizeof ( TCRIMINAL ) == 3 * sizeof ( void * ) + 2 * sizeof ( size_t ) );
    a = nullptr;
    a = createRecord ( "Peter", a );
    a = createRecord ( "John", a );
    a = createRecord ( "Joe", a );
    a = createRecord ( "Maria", a );
    addContact ( a, a -> m_Next );
    addContact ( a -> m_Next -> m_Next, a -> m_Next -> m_Next -> m_Next );
    addContact ( a -> m_Next -> m_Next -> m_Next, a -> m_Next );
    assert ( a && ! strcmp ( a -> m_Name, "Maria" ) && a -> m_Cnt == 1 && a -> m_Contacts[0] == a -> m_Next );
    assert ( a -> m_Next
             && ! strcmp ( a -> m_Next -> m_Name, "Joe" )
             && a -> m_Next -> m_Cnt == 0 );
    assert ( a -> m_Next -> m_Next
             && ! strcmp ( a -> m_Next -> m_Next -> m_Name, "John" )
             && a -> m_Next -> m_Next -> m_Cnt == 1
             && a -> m_Next -> m_Next -> m_Contacts[0] == a -> m_Next -> m_Next -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next
             && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" )
             && a -> m_Next -> m_Next -> m_Next -> m_Cnt == 1
             && a -> m_Next -> m_Next -> m_Next -> m_Contacts[0] == a -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next -> m_Next == nullptr );
    b = cloneList ( a );
    strcpy ( tmp, "Moe" );
    a = createRecord ( tmp, a );
    strcpy ( tmp, "Victoria" );
    a = createRecord ( tmp, a );
    strcpy ( tmp, "Peter" );
    a = createRecord ( tmp, a );
    addContact ( b -> m_Next -> m_Next -> m_Next, b -> m_Next -> m_Next );
    assert ( a
             && ! strcmp ( a -> m_Name, "Peter" )
             && a -> m_Cnt == 0 );
    assert ( a -> m_Next
             && ! strcmp ( a -> m_Next -> m_Name, "Victoria" )
             && a -> m_Next -> m_Cnt == 0 );
    assert ( a -> m_Next -> m_Next
             && ! strcmp ( a -> m_Next -> m_Next -> m_Name, "Moe" )
             && a -> m_Next -> m_Next -> m_Cnt == 0 );
    assert ( a -> m_Next -> m_Next -> m_Next
             && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Name, "Maria" )
             && a -> m_Next -> m_Next -> m_Next -> m_Cnt == 1
             && a -> m_Next -> m_Next -> m_Next -> m_Contacts[0] == a -> m_Next -> m_Next -> m_Next -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next -> m_Next
             && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Joe" )
             && a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Cnt == 0 );
    assert ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
             && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "John" )
             && a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Cnt == 1
             && a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Contacts[0] == a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
             && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" )
             && a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Cnt == 1
             && a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Contacts[0] == a -> m_Next -> m_Next -> m_Next -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == nullptr );
    assert ( b
             && ! strcmp ( b -> m_Name, "Maria" )
             && b -> m_Cnt == 1
             && b -> m_Contacts[0] == b -> m_Next );
    assert ( b -> m_Next
             && ! strcmp ( b -> m_Next -> m_Name, "Joe" )
             && b -> m_Next -> m_Cnt == 0 );
    assert ( b -> m_Next -> m_Next
             && ! strcmp ( b -> m_Next -> m_Next -> m_Name, "John" )
             && b -> m_Next -> m_Next -> m_Cnt == 1
             && b -> m_Next -> m_Next -> m_Contacts[0] == b -> m_Next -> m_Next -> m_Next );
    assert ( b -> m_Next -> m_Next -> m_Next
             && ! strcmp ( b -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" )
             && b -> m_Next -> m_Next -> m_Next -> m_Cnt == 2
             && b -> m_Next -> m_Next -> m_Next -> m_Contacts[0] == b -> m_Next
             && b -> m_Next -> m_Next -> m_Next -> m_Contacts[1] == b -> m_Next -> m_Next );
    assert ( b -> m_Next -> m_Next -> m_Next -> m_Next == nullptr );
    freeList ( a );
    addContact ( b -> m_Next, b -> m_Next );
    a = cloneList ( b );
    assert ( a
             && ! strcmp ( a -> m_Name, "Maria" )
             && a -> m_Cnt == 1
             && a -> m_Contacts[0] == a -> m_Next );
    assert ( a -> m_Next
             && ! strcmp ( a -> m_Next -> m_Name, "Joe" )
             && a -> m_Next -> m_Cnt == 1
             && a -> m_Next -> m_Contacts[0] == a -> m_Next );
    assert ( a -> m_Next -> m_Next
             && ! strcmp ( a -> m_Next -> m_Next -> m_Name, "John" )
             && a -> m_Next -> m_Next -> m_Cnt == 1
             && a -> m_Next -> m_Next -> m_Contacts[0] == a -> m_Next -> m_Next -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next
             && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" )
             && a -> m_Next -> m_Next -> m_Next -> m_Cnt == 2
             && a -> m_Next -> m_Next -> m_Next -> m_Contacts[0] == a -> m_Next
             && a -> m_Next -> m_Next -> m_Next -> m_Contacts[1] == a -> m_Next -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next -> m_Next == nullptr );
    assert ( b
             && ! strcmp ( b -> m_Name, "Maria" )
             && b -> m_Cnt == 1
             && b -> m_Contacts[0] == b -> m_Next );
    assert ( b -> m_Next
             && ! strcmp ( b -> m_Next -> m_Name, "Joe" )
             && b -> m_Next -> m_Cnt == 1
             && b -> m_Next -> m_Contacts[0] == b -> m_Next );
    assert ( b -> m_Next -> m_Next
             && ! strcmp ( b -> m_Next -> m_Next -> m_Name, "John" )
             && b -> m_Next -> m_Next -> m_Cnt == 1
             && b -> m_Next -> m_Next -> m_Contacts[0] == b -> m_Next -> m_Next -> m_Next );
    assert ( b -> m_Next -> m_Next -> m_Next
             && ! strcmp ( b -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" )
             && b -> m_Next -> m_Next -> m_Next -> m_Cnt == 2
             && b -> m_Next -> m_Next -> m_Next -> m_Contacts[0] == b -> m_Next
             && b -> m_Next -> m_Next -> m_Next -> m_Contacts[1] == b -> m_Next -> m_Next );
    assert ( b -> m_Next -> m_Next -> m_Next -> m_Next == nullptr );
    freeList ( b );
    freeList ( a );
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
