#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct TCell {
    struct TCell *m_Right;
    struct TCell *m_Down;
    int m_Row;
    int m_Col;
    int m_Data;
} TCELL;

typedef struct TRowCol {
    struct TRowCol *m_Next;
    TCELL *m_Cells;
    int m_Idx;
} TROWCOL;

typedef struct TSparseMatrix {
    TROWCOL *m_Rows;
    TROWCOL *m_Cols;
} TSPARSEMATRIX;
#endif /* __PROGTEST__ */

void initMatrix(TSPARSEMATRIX *m) {
    m->m_Cols = nullptr;
    m->m_Rows = nullptr;
}

bool findSameIndex(TROWCOL * m, const int index){
    if(m == nullptr){
        return false;
    }
    while(true){
        if(m->m_Idx == index){
            return true;
        }else{
            if(m->m_Next == nullptr){
                return false;
            }else {
                m = m->m_Next;
            }
        }
    }
}

bool findTheSameCell(TROWCOL * r, TROWCOL * c, int row, int col, int value) {
    bool foundRow = false, foundColumn = false;
    if(r == nullptr && c == nullptr){
        return false;
    }
    while (true) {
        if(r != nullptr) {
            if (r->m_Idx == row) {
                foundRow = true;
            } else {
                if (r->m_Next != nullptr) {
                    r = r->m_Next;
                } else {
                    break;
                }
            }
        }
        if(c != nullptr) {
            if (c->m_Idx == col) {
                foundColumn = true;
            } else {
                if (c->m_Next != nullptr) {
                    c = c->m_Next;
                } else {
                    break;
                }
            }
        }
        if(foundRow && foundColumn && r->m_Cells->m_Row == row && r->m_Cells->m_Col == col){
            r->m_Cells->m_Data = value;
            return true;
        }else{
            return false;
        }
    }
    return false;
}


void addSetCell(TSPARSEMATRIX * m, int rowIdx, int colIdx, int value){
    TROWCOL * r = m->m_Rows, * c = m->m_Cols; // берем головные узлы в массиве строк и столбцов

    if(findTheSameCell(r, c, rowIdx, colIdx, value)){ // 1) проверка на то, если такой элемент уже существует
        return;
    }
    /***********************************************************Проверка на ряды************************************************************/
    if(findSameIndex(r, rowIdx)){ // если уже существует ряд с таким индексом
        while(true){ // простая итерация пока не упремся в нужную ячейку с нужным рядом
            if(r->m_Idx == rowIdx){
                break;
            }else{
                r = r->m_Next;
            }
        }
    }else{ // если такая строка еще не существует
        TROWCOL *tmp_row = (TROWCOL *)malloc(sizeof(TROWCOL));
        tmp_row->m_Idx = rowIdx;
        tmp_row->m_Cells = nullptr;
        tmp_row->m_Next = nullptr;

        if(m->m_Rows == nullptr){ // если ничего еще не добавлялось
            m->m_Rows = tmp_row;
            r = m->m_Rows;
        }else {
            if(m->m_Rows->m_Idx > rowIdx) { // проверка на то, если добавляется новая строка, которая может первой строкой
                tmp_row->m_Next = m->m_Rows;
                m->m_Rows = tmp_row;
                r = m->m_Rows;
            }
            // если ряд не может быть первым
            else {
                while (true) {
                    if (r->m_Next != nullptr) {
                        if (rowIdx > r->m_Next->m_Idx) {
                            r = r->m_Next;
                        } else { // значит нашли нужный индекс
                            tmp_row->m_Next = r->m_Next;
                            r->m_Next = tmp_row;
                            r = r->m_Next;
                            break;
                        }
                    } else { // значит добавляемый элемент будет последним
                        r->m_Next = tmp_row;
                        r = r->m_Next;
                        break;
                    }
                }

            }
        }
    }
    /***********************************************************Проверка на ряды закончилась************************************************************/

    /***********************************************************Проверка на столбцы************************************************************/
    if(findSameIndex(c, colIdx)) { // если уже существует столбец с таким индексом
        while(true){ // простая итерация пока не упремся в нужный ячейку с нужным столбцом
            if(c->m_Idx == colIdx){
                break;
            }else{
                c = c->m_Next;
            }
        }
    }else { // если такой столбец еще не существует
        TROWCOL *tmp_col = (TROWCOL *) malloc(sizeof(TROWCOL));
        tmp_col->m_Idx = colIdx;
        tmp_col->m_Cells = nullptr;
        tmp_col->m_Next = nullptr;

        if(m->m_Cols == nullptr){
            m->m_Cols = tmp_col;
            c = m->m_Cols;
        }
        else {
            if (m->m_Cols->m_Idx > colIdx) { // проверка на то, если добавляется новый столбец, который может стать головным либо первым столбцом
                tmp_col->m_Next = m->m_Cols;
                m->m_Cols = tmp_col;
                c = m->m_Cols;
            }
            // если столбец не может быть первым
            else {
                while (true) {
                    if (c->m_Next != nullptr) {
                        if (colIdx > c->m_Next->m_Idx) {
                            c = c->m_Next;
                        } else { // значит нашли нужный индекс
                            tmp_col->m_Next = c->m_Next;
                            c->m_Next = tmp_col;
                            c = c->m_Next;
                            break;
                        }
                    } else { // значит добавляемый элемент будет последним
                        c->m_Next = tmp_col;
                        c = c->m_Next; // ставлю столбец на последнее место и двигаюсь на это место
                        break;
                    }
                }
            }
        }
    }

    /***********************************************************Проверка на столбцы закончилась************************************************************/


    /***********************************************************Проверка на ячейки*************************************************************************/

    TCELL * prvek = (TCELL*)malloc(sizeof(TCELL)), * row_cell = r->m_Cells, * col_cell = c->m_Cells;
    prvek->m_Row = rowIdx; prvek->m_Col = colIdx; prvek->m_Right = nullptr; prvek->m_Down = nullptr; prvek->m_Data = value;

    if(r->m_Cells == nullptr){ // если не добавляли еще ячейку
        r->m_Cells = prvek;
    }else {
        if (r->m_Cells->m_Col > colIdx) { // проверка на то, если может ячейка стоять первой в ряде
            prvek->m_Right = r->m_Cells;
            r->m_Cells = prvek;
        } else {
            while (true) {
                if (row_cell->m_Right != nullptr) {
                    if (prvek->m_Col > row_cell->m_Right->m_Col) {
                        row_cell = row_cell->m_Right;
                    } else {
                        prvek->m_Right = row_cell->m_Right;
                        row_cell->m_Right = prvek;
                        break;
                    }
                } else {
                    row_cell->m_Right = prvek;
                    break;
                }
            }
        }
    }
    if(c->m_Cells == nullptr){
        c->m_Cells = prvek;
    }else {
        if (c->m_Cells->m_Row > prvek->m_Row) {
            prvek->m_Down = c->m_Cells;
            c->m_Cells = prvek;
        } else {
            while (true) {
                if (col_cell->m_Down != nullptr) {
                    if (prvek->m_Row > col_cell->m_Down->m_Row) {
                        col_cell = col_cell->m_Down;
                    } else {
                        prvek->m_Down = col_cell->m_Down;
                        col_cell->m_Down = prvek;
                        break;
                    }
                } else {
                    col_cell->m_Down = prvek;
                    break;
                }
            }
        }
    }
}

TROWCOL* findRow(TSPARSEMATRIX* m, int rowIdx) {
    if (!m || !m->m_Rows) return nullptr;

    TROWCOL* r = m->m_Rows;
    while (r && r->m_Idx != rowIdx) {
        r = r->m_Next;
    }
    return r;
}

TROWCOL* findCol(TSPARSEMATRIX* m, int colIdx) {
    if (!m || !m->m_Cols) return nullptr;

    TROWCOL* c = m->m_Cols;
    while (c && c->m_Idx != colIdx) {
        c = c->m_Next;
    }
    return c;
}

bool removeCellFromRowCol(TROWCOL* row, TROWCOL* col, int rowIdx, int colIdx) {
    if (!row || !col) return false;

    TCELL **prevRow = &(row->m_Cells);
    TCELL *cellRow = row->m_Cells;
    while (cellRow && cellRow->m_Col != colIdx) {
        prevRow = &(cellRow->m_Right);
        cellRow = cellRow->m_Right;
    }
    if (!cellRow) return false;

    TCELL **prevCol = &(col->m_Cells);
    TCELL *cellCol = col->m_Cells;
    while (cellCol && cellCol->m_Row != rowIdx) {
        prevCol = &(cellCol->m_Down);
        cellCol = cellCol->m_Down;
    }
    if (!cellCol) return false;

    *prevRow = cellRow->m_Right;
    *prevCol = cellCol->m_Down;

    if (cellRow != cellCol) {

        return false;
    }

    free(cellRow);
    return true;
}


void deleteRowIfEmpty(TSPARSEMATRIX* m, TROWCOL* row) {
    if (!m || !row || row->m_Cells) return;

    TROWCOL **prev = &(m->m_Rows);
    TROWCOL *curr = m->m_Rows;
    while (curr && curr != row) {
        prev = &(curr->m_Next);
        curr = curr->m_Next;
    }
    if (!curr) return;

    *prev = curr->m_Next;
    free(curr);
}

void deleteColIfEmpty(TSPARSEMATRIX* m, TROWCOL* col) {
    if (!m || !col || col->m_Cells) return;

    TROWCOL **prev = &(m->m_Cols);
    TROWCOL *curr = m->m_Cols;
    while (curr && curr != col) {
        prev = &(curr->m_Next);
        curr = curr->m_Next;
    }
    if (!curr) return;

    *prev = curr->m_Next;
    free(curr);
}

bool removeCell(TSPARSEMATRIX *m, int rowIdx, int colIdx) {
    if (!m) return false;

    TROWCOL* row = findRow(m, rowIdx);
    TROWCOL* col = findCol(m, colIdx);

    if (!row || !col) return false;

    if (!removeCellFromRowCol(row, col, rowIdx, colIdx)) return false;

    deleteRowIfEmpty(m, row);
    deleteColIfEmpty(m, col);

    return true;
}

void freeMatrix(TSPARSEMATRIX *m) {
    TROWCOL * r = m->m_Rows, * c = m->m_Cols;
    if(r != nullptr) {
        while (true) {
            if (r->m_Next == nullptr) {
                while(true){
                    if (r->m_Cells->m_Right == nullptr) {
                        free(r->m_Cells);
                        r->m_Cells = nullptr;
                        break;
                    } else {
                        TCELL *currentCell = r->m_Cells;
                        r->m_Cells = r->m_Cells->m_Right;
                        free(currentCell);
                        currentCell = nullptr;
                    }
                }
                free(r);
                r = nullptr;
                break;
            } else {
                TROWCOL *currentRow = r;
                r = r->m_Next;
                while (true) {
                    if (currentRow->m_Cells->m_Right == nullptr) {
                        free(currentRow->m_Cells);
                        currentRow->m_Cells = nullptr;
                        break;
                    } else {
                        TCELL *currentCell = currentRow->m_Cells;
                        currentRow->m_Cells = currentRow->m_Cells->m_Right;
                        free(currentCell);
                        currentCell = nullptr;
                    }
                }
                free(currentRow);
                currentRow = nullptr;
            }
        }
    }
    if(c != nullptr) {
        while (true) {
            if (c->m_Next == nullptr) {
                free(c);
                c = nullptr;
                break;
            } else {
                TROWCOL *current = c;
                c = c->m_Next;
                free(current);
                current = nullptr;
            }
        }
    }
    m->m_Rows = nullptr;
    m->m_Cols = nullptr;
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
    TSPARSEMATRIX  m;
    initMatrix ( &m );
    addSetCell ( &m, 0, 1, 10 );
    addSetCell ( &m, 1, 0, 20 );
    addSetCell ( &m, 1, 5, 30 );
    addSetCell ( &m, 2, 1, 40 );
    assert ( m . m_Rows
             && m . m_Rows -> m_Idx == 0
             && m . m_Rows -> m_Cells
             && m . m_Rows -> m_Cells -> m_Row == 0
             && m . m_Rows -> m_Cells -> m_Col == 1
             && m . m_Rows -> m_Cells -> m_Data == 10
             && m . m_Rows -> m_Cells -> m_Right == nullptr );
    assert ( m . m_Rows -> m_Next
             && m . m_Rows -> m_Next -> m_Idx == 1
             && m . m_Rows -> m_Next -> m_Cells
             && m . m_Rows -> m_Next -> m_Cells -> m_Row == 1
             && m . m_Rows -> m_Next -> m_Cells -> m_Col == 0
             && m . m_Rows -> m_Next -> m_Cells -> m_Data == 20
             && m . m_Rows -> m_Next -> m_Cells -> m_Right
             && m . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Row == 1
             && m . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Col == 5
             && m . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Data == 30
             && m . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Right == nullptr );
    assert ( m . m_Rows -> m_Next -> m_Next
             && m . m_Rows -> m_Next -> m_Next -> m_Idx == 2
             && m . m_Rows -> m_Next -> m_Next -> m_Cells
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Row == 2
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Col == 1
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Data == 40
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Right == nullptr );
    assert ( m . m_Rows -> m_Next -> m_Next -> m_Next == nullptr );
    assert ( m . m_Cols
             && m . m_Cols -> m_Idx == 0
             && m . m_Cols -> m_Cells
             && m . m_Cols -> m_Cells -> m_Row == 1
             && m . m_Cols -> m_Cells -> m_Col == 0
             && m . m_Cols -> m_Cells -> m_Data == 20
             && m . m_Cols -> m_Cells -> m_Down == nullptr );
    assert ( m . m_Cols -> m_Next
             && m . m_Cols -> m_Next -> m_Idx == 1
             && m . m_Cols -> m_Next -> m_Cells
             && m . m_Cols -> m_Next -> m_Cells -> m_Row == 0
             && m . m_Cols -> m_Next -> m_Cells -> m_Col == 1
             && m . m_Cols -> m_Next -> m_Cells -> m_Data == 10
             && m . m_Cols -> m_Next -> m_Cells -> m_Down
             && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Row == 2
             && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Col == 1
             && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Data == 40
             && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Down == nullptr );
    assert ( m . m_Cols -> m_Next -> m_Next
             && m . m_Cols -> m_Next -> m_Next -> m_Idx == 5
             && m . m_Cols -> m_Next -> m_Next -> m_Cells
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Row == 1
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Col == 5
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Data == 30
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Down == nullptr );
    assert ( m . m_Cols -> m_Next -> m_Next -> m_Next == nullptr );
    assert ( m . m_Rows -> m_Cells == m . m_Cols -> m_Next -> m_Cells );
    assert ( m . m_Rows -> m_Next -> m_Cells == m . m_Cols -> m_Cells );
    assert ( m . m_Rows -> m_Next -> m_Cells -> m_Right == m . m_Cols -> m_Next -> m_Next -> m_Cells );
    assert ( m . m_Rows -> m_Next -> m_Next -> m_Cells == m . m_Cols -> m_Next -> m_Cells -> m_Down );
    addSetCell ( &m, 230, 190, 50 );
    assert ( m . m_Rows
             && m . m_Rows -> m_Idx == 0
             && m . m_Rows -> m_Cells
             && m . m_Rows -> m_Cells -> m_Row == 0
             && m . m_Rows -> m_Cells -> m_Col == 1
             && m . m_Rows -> m_Cells -> m_Data == 10
             && m . m_Rows -> m_Cells -> m_Right == nullptr );
    assert ( m . m_Rows -> m_Next
             && m . m_Rows -> m_Next -> m_Idx == 1
             && m . m_Rows -> m_Next -> m_Cells
             && m . m_Rows -> m_Next -> m_Cells -> m_Row == 1
             && m . m_Rows -> m_Next -> m_Cells -> m_Col == 0
             && m . m_Rows -> m_Next -> m_Cells -> m_Data == 20
             && m . m_Rows -> m_Next -> m_Cells -> m_Right
             && m . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Row == 1
             && m . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Col == 5
             && m . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Data == 30
             && m . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Right == nullptr );
    assert ( m . m_Rows -> m_Next -> m_Next
             && m . m_Rows -> m_Next -> m_Next -> m_Idx == 2
             && m . m_Rows -> m_Next -> m_Next -> m_Cells
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Row == 2
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Col == 1
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Data == 40
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Right == nullptr );
    assert ( m . m_Rows -> m_Next -> m_Next -> m_Next
             && m . m_Rows -> m_Next -> m_Next -> m_Next -> m_Idx == 230
             && m . m_Rows -> m_Next -> m_Next -> m_Next -> m_Cells
             && m . m_Rows -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Row == 230
             && m . m_Rows -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Col == 190
             && m . m_Rows -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Data == 50
             && m . m_Rows -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Right == nullptr );
    assert ( m . m_Rows -> m_Next -> m_Next -> m_Next -> m_Next == nullptr );
    assert ( m . m_Cols
             && m . m_Cols -> m_Idx == 0
             && m . m_Cols -> m_Cells
             && m . m_Cols -> m_Cells -> m_Row == 1
             && m . m_Cols -> m_Cells -> m_Col == 0
             && m . m_Cols -> m_Cells -> m_Data == 20
             && m . m_Cols -> m_Cells -> m_Down == nullptr );
    assert ( m . m_Cols -> m_Next
             && m . m_Cols -> m_Next -> m_Idx == 1
             && m . m_Cols -> m_Next -> m_Cells
             && m . m_Cols -> m_Next -> m_Cells -> m_Row == 0
             && m . m_Cols -> m_Next -> m_Cells -> m_Col == 1
             && m . m_Cols -> m_Next -> m_Cells -> m_Data == 10
             && m . m_Cols -> m_Next -> m_Cells -> m_Down
             && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Row == 2
             && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Col == 1
             && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Data == 40
             && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Down == nullptr );
    assert ( m . m_Cols -> m_Next -> m_Next
             && m . m_Cols -> m_Next -> m_Next -> m_Idx == 5
             && m . m_Cols -> m_Next -> m_Next -> m_Cells
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Row == 1
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Col == 5
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Data == 30
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Down == nullptr );
    assert ( m . m_Cols -> m_Next -> m_Next -> m_Next
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Idx == 190
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Row == 230
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Col == 190
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Data == 50
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Down == nullptr );
    assert ( m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Next == nullptr );
    assert ( m . m_Rows -> m_Cells == m . m_Cols -> m_Next -> m_Cells );
    assert ( m . m_Rows -> m_Next -> m_Cells == m . m_Cols -> m_Cells );
    assert ( m . m_Rows -> m_Next -> m_Cells -> m_Right == m . m_Cols -> m_Next -> m_Next -> m_Cells );
    assert ( m . m_Rows -> m_Next -> m_Next -> m_Cells == m . m_Cols -> m_Next -> m_Cells -> m_Down );
    assert ( m . m_Rows -> m_Next -> m_Next -> m_Next -> m_Cells == m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells );
    assert ( removeCell ( &m, 0, 1 ) );
    assert ( !removeCell ( &m, 0, 1 ) );
    assert ( !removeCell ( &m, 1, 2 ) );
    assert ( m . m_Rows
             && m . m_Rows -> m_Idx == 1
             && m . m_Rows -> m_Cells
             && m . m_Rows -> m_Cells -> m_Row == 1
             && m . m_Rows -> m_Cells -> m_Col == 0
             && m . m_Rows -> m_Cells -> m_Data == 20
             && m . m_Rows -> m_Cells -> m_Right
             && m . m_Rows -> m_Cells -> m_Right -> m_Row == 1
             && m . m_Rows -> m_Cells -> m_Right -> m_Col == 5
             && m . m_Rows -> m_Cells -> m_Right -> m_Data == 30
             && m . m_Rows -> m_Cells -> m_Right -> m_Right == nullptr );
    assert ( m . m_Rows -> m_Next
             && m . m_Rows -> m_Next -> m_Idx == 2
             && m . m_Rows -> m_Next -> m_Cells
             && m . m_Rows -> m_Next -> m_Cells -> m_Row == 2
             && m . m_Rows -> m_Next -> m_Cells -> m_Col == 1
             && m . m_Rows -> m_Next -> m_Cells -> m_Data == 40
             && m . m_Rows -> m_Next -> m_Cells -> m_Right == nullptr );
    assert ( m . m_Rows -> m_Next -> m_Next
             && m . m_Rows -> m_Next -> m_Next -> m_Idx == 230
             && m . m_Rows -> m_Next -> m_Next -> m_Cells
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Row == 230
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Col == 190
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Data == 50
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Right == nullptr );
    assert ( m . m_Rows -> m_Next -> m_Next -> m_Next == nullptr );
    assert ( m . m_Cols
             && m . m_Cols -> m_Idx == 0
             && m . m_Cols -> m_Cells
             && m . m_Cols -> m_Cells -> m_Row == 1
             && m . m_Cols -> m_Cells -> m_Col == 0
             && m . m_Cols -> m_Cells -> m_Data == 20
             && m . m_Cols -> m_Cells -> m_Down == nullptr );
    assert ( m . m_Cols -> m_Next
             && m . m_Cols -> m_Next -> m_Idx == 1
             && m . m_Cols -> m_Next -> m_Cells
             && m . m_Cols -> m_Next -> m_Cells -> m_Row == 2
             && m . m_Cols -> m_Next -> m_Cells -> m_Col == 1
             && m . m_Cols -> m_Next -> m_Cells -> m_Data == 40
             && m . m_Cols -> m_Next -> m_Cells -> m_Down == nullptr );
    assert ( m . m_Cols -> m_Next -> m_Next
             && m . m_Cols -> m_Next -> m_Next -> m_Idx == 5
             && m . m_Cols -> m_Next -> m_Next -> m_Cells
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Row == 1
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Col == 5
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Data == 30
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Down == nullptr );
    assert ( m . m_Cols -> m_Next -> m_Next -> m_Next
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Idx == 190
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Row == 230
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Col == 190
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Data == 50
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Down == nullptr );
    assert ( m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Next == nullptr );
    assert ( m . m_Rows -> m_Cells == m . m_Cols -> m_Cells );
    assert ( m . m_Rows -> m_Cells -> m_Right == m . m_Cols -> m_Next -> m_Next -> m_Cells );
    assert ( m . m_Rows -> m_Next -> m_Cells == m . m_Cols -> m_Next -> m_Cells );
    assert ( m . m_Rows -> m_Next -> m_Next -> m_Cells == m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells );
    freeMatrix ( &m );
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */

