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

typedef struct PairOfCriminals{
    TCRIMINAL * original_crime;
    TCRIMINAL * clone_crime;
}PairOfCriminals;


void initTCRIMINAL(TCRIMINAL * criminal){
    criminal->m_Next = nullptr;
    criminal->m_Name = nullptr;
    criminal->m_Cnt = 0;
    criminal->m_Contacts = nullptr;
    criminal->m_Capacity = 0;
}

void initPairOfCriminals(PairOfCriminals * pair){
    pair->clone_crime = nullptr;
    pair->original_crime = nullptr;
}

void SELF_MADE_STRNCPY(char * destination, const char * source, size_t * destination_capacity){
    size_t counter_for_destination = 0, source_len = strlen(source) + 1;
    for(size_t i = 0; i < source_len; ++i){
        destination[i] = source[i];
        if(counter_for_destination + 1 == *destination_capacity) {
            *destination_capacity *= 2;
            destination = (char *)realloc(destination, *destination_capacity * sizeof(char));
        }
        ++counter_for_destination;
    }
}


TCRIMINAL *createRecord(const char *name, TCRIMINAL *next) {
    if(name == nullptr){
        return next;
    }
    TCRIMINAL *tmp = (TCRIMINAL *) malloc(sizeof(TCRIMINAL));
    initTCRIMINAL(tmp);
    size_t tmp_name = 10;
    tmp->m_Name = (char *) malloc((tmp_name) * sizeof(char));
    SELF_MADE_STRNCPY(tmp->m_Name, name, &tmp_name);
    tmp->m_Next = next;
    tmp->m_Cnt = 0;
    tmp->m_Capacity = 10;
    tmp->m_Contacts = (TCRIMINAL **) malloc(tmp->m_Capacity * sizeof(TCRIMINAL *));
    return tmp;
}


void addContact(TCRIMINAL *dst, TCRIMINAL *contact) {
    if(dst == nullptr || contact == nullptr){
        return;
    }
    dst->m_Contacts[dst->m_Cnt] = contact;
    if(dst->m_Cnt + 1 == dst->m_Capacity) {
        dst->m_Capacity *= 2;
        dst->m_Contacts = (TCRIMINAL **)realloc(dst->m_Contacts, dst->m_Capacity * sizeof(TCRIMINAL *));
    }
    ++dst->m_Cnt;
}


TCRIMINAL * cloneList(TCRIMINAL *node){
    TCRIMINAL * head = nullptr, * tmp_node = node, * tail = nullptr;
    size_t size_for_pairs = 10;
    PairOfCriminals * pairs = (PairOfCriminals*)malloc(size_for_pairs * sizeof(PairOfCriminals));
    size_t counter_for_pairs = 0;
    while(true){
        if(tmp_node == nullptr){
            break;
        }else{
            TCRIMINAL * clone_criminal = (TCRIMINAL*)malloc(sizeof(TCRIMINAL));
            initTCRIMINAL(clone_criminal);
            size_t tmp_name = 10;
            clone_criminal->m_Name = (char*)malloc((tmp_name) * sizeof(char));
            SELF_MADE_STRNCPY(clone_criminal->m_Name, tmp_node->m_Name, &tmp_name);
            clone_criminal->m_Capacity = node->m_Capacity;

            if(tail == nullptr){
                head = clone_criminal;
            }else{
                tail->m_Next = clone_criminal;
            }
            tail = clone_criminal;
            initPairOfCriminals(&pairs[counter_for_pairs]);
            pairs[counter_for_pairs].original_crime = tmp_node;
            pairs[counter_for_pairs].clone_crime = clone_criminal;
            if(counter_for_pairs + 1 == size_for_pairs){
                size_for_pairs *= 2;
                pairs = (PairOfCriminals*)realloc(pairs, size_for_pairs * sizeof(PairOfCriminals));
            }
            ++counter_for_pairs;
            tmp_node = tmp_node->m_Next;
        }
    }
    tmp_node = node;
    TCRIMINAL * tmp_new_node = head;
    while(true){
        if(tmp_node == nullptr){
            break;
        }else{
            tmp_new_node->m_Contacts = (TCRIMINAL**)malloc(tmp_new_node->m_Capacity * sizeof(TCRIMINAL*));
            if(tmp_node->m_Cnt != 0){
                for(size_t i = 0; i < tmp_node->m_Cnt; ++i){
                    int iterator = 0;
                    while(true){
                        if(pairs[iterator].original_crime == tmp_node->m_Contacts[i]){
                            addContact(tmp_new_node, pairs[iterator].clone_crime);
                            break;
                        }else{
                            ++iterator;
                        }
                        if(iterator > counter_for_pairs){
                            break;
                        }
                    }
                }
            }
        }
        tmp_node = tmp_node->m_Next;
        tmp_new_node = tmp_new_node->m_Next;
    }
    free(pairs);
    return head;
}


void freeList(TCRIMINAL *src) {
    if(src == nullptr){
        return;
    }
    while (src != nullptr) {
        TCRIMINAL *current = src;
        src = src->m_Next;

        // Free contacts array
        free(current->m_Contacts);

        // Free name string
        free(current->m_Name);

        // Free current node
        free(current);
    }
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
    b = cloneList( a );
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
