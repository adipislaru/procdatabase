/*
 * DataBase Simulator
 * ----------------------------
 *      Autor: Pislaru Costin-Adrian
 *
 *      Cerinte:
 *
 *      - Sa se afiseze toate chitarele ce au acordaj tip: "standard".
 *      - Sa se introduca de la tastatura numele si prenumele unui patron,
 *          si sa se afiseze adresa magazinului cel mai profitabil pe care il detine.
 *      - Sa se verifice disponibilitatea unui Brand de chitara in magazinele din baza de date.
 *
 *      03/12/2016
 * ----------------------------
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_MAX 128
#define STR_MAX_LOW 64

// Definirea tipurilor de date necesare

typedef struct // Tabelul modele al bazei de date.
{
    int id, tip, corzi, pret;
    char nume[ STR_MAX ], brand[ STR_MAX ], acordaj[ STR_MAX ];
} modele;

typedef struct // Tabelul magazine al bazei de date.
{
    int id, id_patron, data;
    char adresa[ STR_MAX ];
} magazine;

typedef struct // Tabelul stock al bazei de date.
{
    int id, id_magazin, id_model, numar;
} stock;

typedef struct // Tabelul patroni al bazei de date.
{
    int id, id_magazin;
    char nume[ STR_MAX_LOW ], prenume[ STR_MAX_LOW ], tel[ STR_MAX_LOW ];
} patroni;

typedef struct // Tabelul facturi al bazei de date.
{
    int id, id_magazin, id_model, data, achitat;
} facturi;

typedef struct // Structura driverului de incrementare.
{
    int modele, magazine, stock, patroni, facturi;
} auto_increment;

typedef struct // Structura pentru cautarea extinsa.
{
    int *vars, num;
} db_query_extended;

typedef struct // Structura driverului bazei de date.
{
    modele *modele;
    magazine *magazine;
    stock *stock;
    patroni *patroni;
    facturi *facturi;

    /* declararea vectorului ce contine:
        pointeri catre procedurile realizabile pe driverul bazei de date */

    void( **manipulate )( );
} DataBase;

typedef union // Structura pentru cautare.
{
    modele *query_modele;
    magazine *query_magazine;
    stock *query_stock;
    patroni *query_patroni;
    facturi *query_facturi;
} db_query_union;

typedef enum // Enumerarea actiunilor pe tabel.
{
	act_on_modele = 0,
	act_on_magazine,
	act_on_stock,
	act_on_patroni,
	act_on_facturi
} act_types;

typedef enum // Enumerarea tipurilor de calcul.
{
    minim = 0,
    maxim,
    average
} calc_types;

enum // Enumerarea procedurilor realizabile pe driverul bazei de date.
{
    _ = -1,
    action_insert,
    action_remove,
    action_update,
    action_query,
    action_query_ex,
    action_sort,
    action_calculate
};

// Functii auxiliare

int sort_modele_comp( const void *a, const void *b )
{
    /*
     * Functie: sort_modele_comp
     * --------------------
     * Realizeaza comparatia dintre doua elemente ale unei structuri
     *
     *  a, b: elemente
     *
     *  return: 1 ( daca a > b ), 0( daca a = b ), -1( daca a < b ) | tip int
     */

    modele *oa = a;
    modele *ob = b;

    return( strcmp( oa->nume, ob->nume ) );
}

// Procedurile bazei de date

void db_insert( DataBase *db, auto_increment *a_i, act_types db_insert_type,... )
{
    /*
     * Functie: db_insert( procedura )
     * --------------------
     * Realizeaza inserarea de inregistrari in tabelele bazei de date.
     *
     *  db: pointer catre driverul bazei de date;
     *  a_i: pointer catre driverul de incrementare al bazei de date;
     *  db_insert_type: tipul tabelului in care se insereaza;
     *  ...: valorile inserate, specifice campurilor tabelului;
     *
     *  nu returneaza nimic.
     */

    va_list fArgs; // lista de argumente( functia are numar variabil de argumente )

    switch( db_insert_type )
    {
        case act_on_modele:
        {
            va_start( fArgs, 6 );

            db->modele[ a_i->modele - 1 ].id = a_i->modele - 1;
            db->modele[ a_i->modele - 1 ].tip = va_arg( fArgs, int );
            db->modele[ a_i->modele - 1 ].corzi = va_arg( fArgs, int );
            db->modele[ a_i->modele - 1 ].pret = va_arg( fArgs, int );
            strcpy( db->modele[ a_i->modele - 1 ].nume, va_arg( fArgs, char * ) );
            strcpy( db->modele[ a_i->modele - 1 ].brand, va_arg( fArgs, char * ) );
            strcpy( db->modele[ a_i->modele - 1 ].acordaj, va_arg( fArgs, char * ) );

            va_end( fArgs );

            db->modele = realloc( db->modele, ( ++a_i->modele ) * sizeof( modele ) );

            break;
        }

        case act_on_magazine:
        {
            va_start( fArgs, 3 );

            db->magazine[ a_i->magazine - 1 ].id = a_i->magazine - 1;
            db->magazine[ a_i->magazine - 1 ].id_patron = va_arg( fArgs, int );
            db->magazine[ a_i->magazine - 1 ].data = va_arg( fArgs, int );
            strcpy( db->magazine[ a_i->magazine - 1 ].adresa, va_arg( fArgs, char * ) );

            va_end( fArgs );

            db->magazine = realloc( db->magazine, ( ++a_i->magazine ) * sizeof( magazine ) );

            break;
        }

        case act_on_stock:
        {
            va_start( fArgs, 3 );

            db->stock[ a_i->stock - 1 ].id = a_i->stock - 1;
            db->stock[ a_i->stock - 1 ].id_magazin = va_arg( fArgs, int );
            db->stock[ a_i->stock - 1 ].id_model = va_arg( fArgs, int );
            db->stock[ a_i->stock - 1 ].numar = va_arg( fArgs, int );

            va_end( fArgs );

            db->stock = realloc( db->stock, ( ++a_i->stock ) * sizeof( stock ) );

            break;
        }

        case act_on_patroni:
        {
            va_start( fArgs, 4 );

            db->patroni[ a_i->patroni - 1 ].id = a_i->patroni - 1;
            db->patroni[ a_i->patroni - 1 ].id_magazin = va_arg( fArgs, int );
            strcpy( db->patroni[ a_i->patroni - 1 ].nume, va_arg( fArgs, char * ) );
            strcpy( db->patroni[ a_i->patroni - 1 ].prenume, va_arg( fArgs, char * ) );
            strcpy( db->patroni[ a_i->patroni - 1 ].tel, va_arg( fArgs, char * ) );

            va_end( fArgs );

            db->patroni = realloc( db->patroni, ( ++a_i->patroni ) * sizeof( patroni ) );

            break;
        }

        case act_on_facturi:
        {
            va_start( fArgs, 4 );

            db->facturi[ a_i->facturi - 1 ].id = a_i->facturi - 1;
            db->facturi[ a_i->facturi - 1 ].id_magazin = va_arg( fArgs, int );
            db->facturi[ a_i->facturi - 1 ].id_model = va_arg( fArgs, int );
            db->facturi[ a_i->facturi - 1 ].data = va_arg( fArgs, int );
            db->facturi[ a_i->facturi - 1 ].achitat = va_arg( fArgs, int );

            va_end( fArgs );

            db->facturi = realloc( db->facturi, ( ++a_i->facturi ) * sizeof( facturi ) );

            break;
        }
    }
}

void db_remove( DataBase *db, auto_increment *a_i, act_types db_remove_type, int removeId )
{
    /*
     * Functie: db_remove( procedura )
     * --------------------
     * Realizeaza stergerea de inregistrari din tabelele bazei de date.
     *
     *  db: pointer catre driverul bazei de date;
     *  a_i: pointer catre driverul de incrementare al bazei de date;
     *  db_remove_type: tipul tabelului in care se insereaza;
     *  removeId: id-ul inregistrarii ce trebuie stearsa;
     *
     *  nu returneaza nimic.
     */

    int i;

    switch( db_remove_type )
    {
        case act_on_modele:
        {
            if( a_i->modele == 2 )
            {
                free( &db->modele[ 0 ] );
                free( db->modele );

                db->modele = malloc( a_i->modele * sizeof( modele ) );

                a_i->modele--;

                break;
            }
            else if( removeId == a_i->modele - 1 )
            {
                free( &db->modele[ a_i->modele - 1 ] );

                a_i->modele--;

                break;
            }
            else
            {
                for( i = removeId; i < a_i->modele - 1 ; i++ )
                {
                    db->modele[ i ] = db->modele[ i + 1 ];
                    db->modele[ i ].id--;
                }

                free( &db->modele[ a_i->modele - 1 ] );

                a_i->modele--;

                break;
            }

            break;
        }

        case act_on_magazine:
        {
            if( a_i->magazine == 2 )
            {
                free( &db->magazine[ 0 ] );
                free( db->magazine );

                db->magazine = malloc( a_i->magazine * sizeof( magazine ) );

                a_i->magazine--;

                break;
            }
            else if( removeId == a_i->magazine - 1 )
            {
                free( &db->magazine[ a_i->magazine - 1 ] );

                a_i->magazine--;

                break;
            }
            else
            {
                for( i = removeId; i < a_i->magazine - 1 ; i++ )
                {
                    db->magazine[ i ] = db->magazine[ i + 1 ];
                    db->magazine[ i ].id--;
                }

                free( &db->magazine[ a_i->magazine - 1 ] );

                a_i->magazine--;

                break;
            }

            break;
        }

        case act_on_stock:
        {
            if( a_i->stock == 2 )
            {
                free( &db->stock[ 0 ] );
                free( db->stock );

                db->stock = malloc( a_i->stock * sizeof( stock ) );

                a_i->stock--;

                break;
            }
            else if( removeId == a_i->stock - 1 )
            {
                free( &db->stock[ a_i->stock - 1 ] );

                a_i->stock--;

                break;
            }
            else
            {
                for( i = removeId; i < a_i->stock - 1 ; i++ )
                {
                    db->stock[ i ] = db->stock[ i + 1 ];
                    db->stock[ i ].id--;
                }

                free( &db->stock[ a_i->stock - 1 ] );

                a_i->stock--;

                break;
            }

            break;
        }

        case act_on_patroni:
        {
            if( a_i->patroni == 2 )
            {
                free( &db->patroni[ 0 ] );
                free( db->patroni );

                db->patroni = malloc( a_i->patroni * sizeof( patroni ) );

                a_i->patroni--;

                break;
            }
            else if( removeId == a_i->patroni - 1 )
            {
                free( &db->patroni[ a_i->patroni - 1 ] );

                a_i->patroni--;

                break;
            }
            else
            {
                for( i = removeId; i < a_i->patroni - 1 ; i++ )
                {
                    db->patroni[ i ] = db->patroni[ i + 1 ];
                    db->patroni[ i ].id--;
                }

                free( &db->patroni[ a_i->patroni - 1 ] );

                a_i->patroni--;

                break;
            }

            break;
        }

        case act_on_facturi:
        {
            if( a_i->facturi == 2 )
            {
                free( &db->facturi[ 0 ] );
                free( db->facturi );

                db->facturi = malloc( a_i->facturi * sizeof( facturi ) );

                a_i->facturi--;

                break;
            }
            else if( removeId == a_i->facturi - 1 )
            {
                free( &db->facturi[ a_i->facturi - 1 ] );

                a_i->facturi--;

                break;
            }
            else
            {
                for( i = removeId; i < a_i->facturi - 1 ; i++ )
                {
                    db->facturi[ i ] = db->facturi[ i + 1 ];
                    db->facturi[ i ].id--;
                }

                free( &db->facturi[ a_i->facturi - 1 ] );

                a_i->facturi--;

                break;
            }

            break;
        }
    }
}

void db_update( DataBase *db, act_types db_update_type, int upId,... )
{
    /*
     * Functie: db_update( procedura )
     * --------------------
     * Realizeaza modificarea de inregistrari din tabelele bazei de date.
     *
     *  db: pointer catre driverul bazei de date;
     *  db_update_type: tipul tabelului in care se insereaza;
     *  upId: id-ul inregistrarii ce trebuie modificata;
     *
     *  nu returneaza nimic.
     */

    va_list fArgs; // lista de argumente( functia are numar variabil de argumente )

    switch( db_update_type )
    {
        case act_on_modele:
        {
            va_start( fArgs, 6 );

            int tip, corzi, pret;
            char nume[ STR_MAX ], brand[ STR_MAX ], acordaj[ STR_MAX ];

            tip = va_arg( fArgs, int );
            corzi = va_arg( fArgs, int );
            pret = va_arg( fArgs, int );
            strcpy( nume, va_arg( fArgs, char* ) );
            strcpy( brand, va_arg( fArgs, char* ) );
            strcpy( acordaj, va_arg( fArgs, char* ) );

            if( tip != _ ) db->modele[ upId ].tip = tip;
            if( corzi != _ ) db->modele[ upId ].corzi = corzi;
            if( pret != _ ) db->modele[ upId ].pret = pret;
            if( strlen( nume ) > 0 ) strcpy( db->modele[ upId ].nume, nume );
            if( strlen( brand ) > 0 ) strcpy( db->modele[ upId ].brand, brand );
            if( strlen( acordaj ) > 0 ) strcpy( db->modele[ upId ].acordaj, acordaj );

            va_end( fArgs );

            break;
        }

        case act_on_magazine:
        {
            va_start( fArgs, 3 );

            int id_patron, data;
            char adresa[ STR_MAX ];

            id_patron = va_arg( fArgs, int );
            data = va_arg( fArgs, int );
            strcpy( adresa, va_arg( fArgs, char* ) );

            if( id_patron != _ ) db->magazine[ upId ].id_patron = id_patron;
            if( data != _ ) db->magazine[ upId ].data = data;
            if( strlen( adresa ) > 0 ) strcpy( db->magazine[ upId ].adresa, adresa );

            va_end( fArgs );

            break;
        }

        case act_on_stock:
        {
            va_start( fArgs, 3 );

            int id_magazin, id_model, numar;

            id_magazin = va_arg( fArgs, int );
            id_model = va_arg( fArgs, int );
            numar = va_arg( fArgs, int );

            if( id_magazin != _ ) db->stock[ upId ].id_magazin = id_magazin;
            if( id_model != _ ) db->stock[ upId ].id_model = id_model;
            if( numar != _ ) db->stock[ upId ].numar = numar;

            va_end( fArgs );

            break;
        }

        case act_on_patroni:
        {
            va_start( fArgs, 4 );

            int id_magazin;
            char nume[ STR_MAX_LOW ], prenume[ STR_MAX_LOW ], tel[ STR_MAX_LOW ];

            id_magazin = va_arg( fArgs, int );
            strcpy( nume, va_arg( fArgs, char* ) );
            strcpy( prenume, va_arg( fArgs, char* ) );
            strcpy( tel, va_arg( fArgs, char* ) );

            if( id_magazin != _ ) db->patroni[ upId ].id_magazin = id_magazin;
            if( strlen( nume ) > 0 ) strcpy( db->patroni[ upId ].nume, nume );
            if( strlen( prenume ) > 0 ) strcpy( db->patroni[ upId ].prenume, prenume );
            if( strlen( tel ) > 0 ) strcpy( db->patroni[ upId ].tel, tel );

            va_end( fArgs );

            break;
        }

        case act_on_facturi:
        {
            va_start( fArgs, 4 );

            int id_magazin, id_model, data, achitat;

            id_magazin = va_arg( fArgs, int );
            id_model = va_arg( fArgs, int );
            data = va_arg( fArgs, int );
            achitat = va_arg( fArgs, int );

            if( id_magazin != _ ) db->facturi[ upId ].id_magazin = id_magazin;
            if( id_model != _ ) db->facturi[ upId ].id_model = id_model;
            if( data != _ ) db->facturi[ upId ].data = data;
            if( achitat != _ ) db->facturi[ upId ].achitat = achitat;

            va_end( fArgs );

            break;
        }
    }
}

db_query_union db_query( DataBase *db, act_types db_query_type, int queryId )
{
    /*
     * Functie: db_query( procedura )
     * --------------------
     * Realizeaza selectarea de inregistrari din tabelele bazei de date.
     *
     *  db: pointer catre driverul bazei de date;
     *  db_query_type: tipul tabelului din care se selecteaza;
     *  queryId: id-ul inregistrarii ce trebuie selectata;
     *
     *  returneaza o variabila de tip Union ce contine pointeri catre rezultat
     *  in functie de cautarea aleasa.
     *     e.g. : {Variabila}.query_modele ( pointer catre o structura de tip modele ).
     */

    db_query_union Value;

    switch( db_query_type )
    {
        case act_on_modele:
        {
            Value.query_modele = &db->modele[ queryId ];

            break;
        }

        case act_on_magazine:
        {
            Value.query_magazine = &db->magazine[ queryId ];

            break;
        }

        case act_on_stock:
        {
            Value.query_stock = &db->stock[ queryId ];

            break;
        }

        case act_on_patroni:
        {
            Value.query_patroni = &db->patroni[ queryId ];

            break;
        }

        case act_on_facturi:
        {
            Value.query_facturi = &db->facturi[ queryId ];

            break;
        }
    }

    return Value;
}

db_query_extended db_query_ex( DataBase *db, auto_increment *a_i, act_types db_query_type,... )
{
    /*
     * Functie: db_query_ex( procedura )
     * --------------------
     * Realizeaza selectarea extinsa de inregistrari din tabelele bazei de date.
     *
     *  db: pointer catre driverul bazei de date;
     *  a_i: pointer catre driverul de incrementare al bazei de date;
     *  db_query_type: tipul tabelului din care se selecteaza;
     *  ...: valori specifice campurilor inregistrarilor ce sunt cautate :
     *          daca nu se doreste cautarea in functie de o variabila tip INT, se trece _
     *          daca nu se doreste cautarea in functie de o variabila de tip CHAR, se trece ""
     *
     * return: returneaza o structura de tip db_query_extended ce contine un array si un int.
     *         {variabila}.num (int) -> numarul inregistrarilor gasite
     *         {variabila}.vars (int array ) -> array ce contine id-urile inregistrarilor gasite
     *
     */

    db_query_extended localQuery;

    int i;

    va_list fArgs; // lista de argumente( functia are numar variabil de argumente )

    localQuery.num = 1;
    localQuery.vars = malloc( sizeof( int ) );

    switch( db_query_type )
    {
        case act_on_modele:
        {
            va_start( fArgs, 6 );

            int tip, corzi, pret;
            char nume[ STR_MAX ], brand[ STR_MAX ], acordaj[ STR_MAX ];

            tip = va_arg( fArgs, int );
            corzi = va_arg( fArgs, int );
            pret = va_arg( fArgs, int );
            strcpy( nume, va_arg( fArgs, char* ) );
            strcpy( brand, va_arg( fArgs, char* ) );
            strcpy( acordaj, va_arg( fArgs, char* ) );

            //verificarea parametrilor peste care se trece

            for( i = 0; i < a_i->modele - 1; i++ )
                if(
                       ( ( tip != _ ) ? ( db->modele[ i ].tip == tip ) : 1 ) &&
                       ( ( corzi != _ ) ? ( db->modele[ i ].corzi == corzi ) : 1 ) &&
                       ( ( pret != _ ) ? ( db->modele[ i ].pret == pret ) : 1 ) &&
                       ( ( strlen( nume ) > 0 ) ? ( strcmp( db->modele[ i ].nume, nume ) == 0 ) : 1 ) &&
                       ( ( strlen( brand ) > 0 ) ? ( strcmp( db->modele[ i ].brand, brand ) == 0 ) : 1 ) &&
                       ( ( strlen( acordaj ) > 0 ) ? ( strcmp( db->modele[ i ].acordaj, acordaj ) == 0 ) : 1 )
                   )
                {
                    localQuery.vars[ localQuery.num - 1 ] = i;

                    localQuery.num++;

                    localQuery.vars = realloc( localQuery.vars, localQuery.num * sizeof( int ) );
                }

            localQuery.num--;

            va_end( fArgs );

            return localQuery;
        }

        case act_on_magazine:
        {
            va_start( fArgs, 3 );

            int id_patron, data;
            char adresa[ STR_MAX ];

            id_patron = va_arg( fArgs, int );
            data = va_arg( fArgs, int );
            strcpy( adresa, va_arg( fArgs, char* ) );

            //verificarea parametrilor peste care se trece

            for( i = 0; i < a_i->magazine - 1; i++ )
                if(
                       ( ( id_patron != _ ) ? ( db->magazine[ i ].id_patron == id_patron ) : 1 ) &&
                       ( ( data != _ ) ? ( db->magazine[ i ].data == data ) : 1 ) &&
                       ( ( strlen( adresa ) > 0 ) ? ( strcmp( db->magazine[ i ].adresa, adresa ) == 0 ) : 1 )
                   )
                {
                    localQuery.vars[ localQuery.num - 1 ] = i;

                    localQuery.num++;

                    localQuery.vars = realloc( localQuery.vars, localQuery.num * sizeof( int ) );
                }

            localQuery.num--;

            va_end( fArgs );

            return localQuery;
        }

        case act_on_stock:
        {
            va_start( fArgs, 3 );

            int id_magazin, id_model, numar;

            id_magazin = va_arg( fArgs, int );
            id_model = va_arg( fArgs, int );
            numar = va_arg( fArgs, int );

            //verificarea parametrilor peste care se trece

            for( i = 0; i < a_i->stock - 1; i++ )
                if(
                       ( ( id_magazin != _ ) ? ( db->stock[ i ].id_magazin == id_magazin ) : 1 ) &&
                       ( ( id_model != _ ) ? ( db->stock[ i ].id_model == id_model ) : 1 ) &&
                       ( ( numar != _ ) ? ( db->stock[ i ].numar == numar ) : 1 )
                   )
                {
                    localQuery.vars[ localQuery.num - 1 ] = i;

                    localQuery.num++;

                    localQuery.vars = realloc( localQuery.vars, localQuery.num * sizeof( int ) );
                }

            localQuery.num--;

            va_end( fArgs );

            return localQuery;
        }

        case act_on_patroni:
        {
            va_start( fArgs, 4 );

            int id_magazin;
            char nume[ STR_MAX_LOW ], prenume[ STR_MAX_LOW ], tel[ STR_MAX_LOW ];

            id_magazin = va_arg( fArgs, int );
            strcpy( nume, va_arg( fArgs, char* ) );
            strcpy( prenume, va_arg( fArgs, char* ) );
            strcpy( tel, va_arg( fArgs, char* ) );

            //verificarea parametrilor peste care se trece

            for( i = 0; i < a_i->patroni - 1; i++ )
                if(
                       ( ( id_magazin != _ ) ? ( db->patroni[ i ].id_magazin == id_magazin ) : 1 ) &&
                       ( ( strlen( nume ) > 0 ) ? ( strcmp( db->patroni[ i ].nume, nume ) == 0 ) : 1 ) &&
                       ( ( strlen( prenume ) > 0 ) ? ( strcmp( db->patroni[ i ].prenume, prenume ) == 0 ) : 1 ) &&
                       ( ( strlen( tel ) > 0 ) ? ( strcmp( db->patroni[ i ].tel, tel ) == 0 ) : 1 )
                   )
                {
                    localQuery.vars[ localQuery.num - 1 ] = i;

                    localQuery.num++;

                    localQuery.vars = realloc( localQuery.vars, localQuery.num * sizeof( int ) );
                }

            localQuery.num--;

            va_end( fArgs );

            return localQuery;
        }

        case act_on_facturi:
        {
            va_start( fArgs, 4 );

            int id_magazin, id_model, data, achitat;

            id_magazin = va_arg( fArgs, int );
            id_model = va_arg( fArgs, int );
            data = va_arg( fArgs, int );
            achitat = va_arg( fArgs, int );

            //verificarea parametrilor peste care se trece

            for( i = 0; i < a_i->facturi - 1; i++ )
                if(
                       ( ( id_magazin != _ ) ? ( db->facturi[ i ].id_magazin == id_magazin ) : 1 ) &&
                       ( ( id_model != _ ) ? ( db->facturi[ i ].id_model == id_model ) : 1 ) &&
                       ( ( data != _ ) ? ( db->facturi[ i ].data == data ) : 1 ) &&
                       ( ( achitat != _ ) ? ( db->facturi[ i ].achitat == achitat ) : 1 )
                   )
                {
                    localQuery.vars[ localQuery.num - 1 ] = i;

                    localQuery.num++;

                    localQuery.vars = realloc( localQuery.vars, localQuery.num * sizeof( int ) );
                }

            localQuery.num--;

            va_end( fArgs );

            return localQuery;
        }
    }
};

void db_sort( DataBase *db, auto_increment *a_i, act_types db_sort_type )
{
    /*
     * Functie: db_sort( procedura )
     * --------------------
     * Realizeaza sortarea inregistrarilor din tabelele bazei de date.
     *
     *  db: pointer catre driverul bazei de date;
     *  a_i: pointer catre driverul de incrementare al bazei de date;
     *  db_sort_type: tipul tabelului ce urmeaza a fi sortat;
     *
     *  nu returneaza nimic.
     *
     */

    int i;

    switch( db_sort_type )
    {
        case act_on_modele:
        {
            qsort( db->modele, a_i->modele - 1, sizeof( modele ), sort_modele_comp );

            for( i = 0; i < a_i->modele - 1; i++ ) // corectarea id-urilor interne, dupa sortare
                db->modele[ i ].id = i;

            break;
        }
    }
}

int db_calculate( DataBase *db, auto_increment *a_i, calc_types calc_type )
{
    /*
     * Functie: db_calculate( procedura )
     * --------------------
     * Realizeaza calcule matematice pe inregistrarile din tabelul "modele" al bazei de date.
     *
     *  db: pointer catre driverul bazei de date;
     *  a_i: pointer catre driverul de incrementare al bazei de date;
     *  db_calc_type: tipul calculului realizat
     *
     *  return: valoarea rezultata in urma calculului | tip int
     *
     */

    int i, min = db->modele[ 0 ].pret, max = db->modele[ 0 ].pret, avg = 0;

    switch( calc_type )
    {
        case minim:
        {
            for( i = 1; i < a_i->modele - 1; i++ )
                if( db->modele[ i ].pret < min )
                    min = db->modele[ i ].pret;

            return min;
        }

        case maxim:
        {
            for( i = 1; i < a_i->modele - 1; i++ )
                if( db->modele[ i ].pret > max )
                    max = db->modele[ i ].pret;

            return max;
        }

        case average:
        {
            for( i = 0; i < a_i->modele - 1; i++ )
                avg += db->modele[ i ].pret;

            return avg/( a_i->modele - 1 );
        }
    }
}

/* Definitii pentru cast, folosite pentru lizibilitate */

typedef void ( *cast_insert )( DataBase *, auto_increment *, act_types,... );
typedef void ( *cast_remove )( DataBase *, auto_increment *, act_types, int );
typedef void ( *cast_update )( DataBase *, act_types, int,... );
typedef db_query_union ( *cast_query )( DataBase *, act_types, int );
typedef db_query_extended ( *cast_query_ex )( DataBase *, auto_increment *, act_types,... );
typedef void ( *cast_sort )( DataBase *, auto_increment *, act_types );
typedef int ( *cast_calculate )( DataBase *, auto_increment *, calc_types );

// Functii auxiliare bazei de date

void db_free_query_ex( db_query_extended *dbq )
{
    /*
     * Functie: db_free_query_ex
     * --------------------
     * Realizeaza eliberarea memoriei alocate dinamic pentru cautarea extinsa.
     *
     *  dbq: pointer catre cautarea extinsa.
     *
     *  nu returneaza nimic.
     *
     */

    free( dbq->vars );
}

void db_free_mem( DataBase *db )
{
    /*
     * Functie: db_free_mem
     * --------------------
     * Realizeaza eliberarea memoriei alocate dinamic pentru baza de date.
     *
     *  db: pointer catre driverul bazei de date.
     *
     *  nu returneaza nimic.
     *
     */

    free( db->modele );
    free( db->magazine );
    free( db->facturi );
    free( db->stock );
    free( db->patroni );
}

void db_initiate( DataBase *db, auto_increment *a_i )
{
    /*
     * Functie: db_initiate
     * --------------------
     * Realizeaza initierea si configurarea driverului bazei de date si al incrementarii.
     *
     *  db: pointer catre driverul bazei de date;
     *  a_i: pointer catre driverul de incrementare al bazei de date;
     *
     *  nu returneaza nimic.
     *
     */

    // initializarea driverului de incrementarea cu valori de 1
    // prima inregistrare este mereu "gata de a fi folosita", dar goala
    // dupa fiecare inserare, se aloca memorie pentru inca o inregistrare, goala

    a_i->modele = a_i->magazine = a_i->stock = a_i->patroni = a_i->facturi = 1;

    // alocarea memoriei pentru inregistrarile din tabelele bazei de date

    db->modele = malloc( a_i->modele * sizeof( modele ) );
    db->magazine = malloc( a_i->magazine * sizeof( magazine ) );
    db->stock = malloc( a_i->stock * sizeof( stock ) );
    db->patroni = malloc( a_i->patroni * sizeof( patroni ) );
    db->facturi = malloc( a_i->facturi * sizeof( facturi ) );

    // initializarea vectorului de proceduri( pointeri )
    // fiecare item al vectorului este castat catre un tip generic

    db->manipulate = malloc( 7 * sizeof( void( * )( ) ) );
    db->manipulate[ action_insert ] = ( void( * )( ) ) &db_insert;
    db->manipulate[ action_remove ] = ( void( * )( ) ) &db_remove;
    db->manipulate[ action_update ] = ( void( * )( ) ) &db_update;
    db->manipulate[ action_query ] = ( void( * )( ) ) &db_query;
    db->manipulate[ action_query_ex ] = ( void( * )( ) ) &db_query_ex;
    db->manipulate[ action_sort ] = ( void( * )( ) ) &db_sort;
    db->manipulate[ action_calculate ] = ( void( * )( ) ) &db_calculate;
}

/* I N S T R U C T I U N I    D E    F O L O S I R E

    Lista tabele si campuri:
        - modele : ( auto_increment id ), int tip, int corzi, int pret, char nume, char brand, char acordaj
        - magazine : ( auto_increment id ), int id_patron, int data, char adresa
        - stock : ( auto_increment id ), int id_magazin, int id_model, int numar
        - patroni : ( auto_increment id ), int id_magazin, char nume, char prenume, char tel
        - facturi : ( auto_increment id ), int id_magazin, int id_model, int data, int achitat

    Lista de actiuni:
        - insert
        - remove
        - update
        - query
        - query_ex
        - sort
        - calculate

    Lista de calcule:
        - minim
        - maxim
        - average

    Lista { PARAMS } :
        - parametrii sunt campurile de tabel, listate mai sus.

    Tipuri de actiuni asupra tabelelor:
        - act_on_{TABEL} -> unde {TABEL} face parte din "Lista de tabele".

    Tipuri de actiuni:
        - action_{ACTIUNE} -> unde {ACTIUNE} face parte din "Lista de actiuni".

    Tipuri de cast:
        - cast_{ACTIUNE} -> unde {ACTIUNE} face parte din "Lista de actiuni".

    Exemple pentru manipularea bazei de date:
        Functie generica: ( ( TIP_CAST )db.manipulate[ TIP_ACTIUNE ] )( { PARAMETRII } )

    - insert : ( ( cast_insert )db.manipulate[ action_insert ] )( &db, &a_i, act_on_{TABEL}, { PARAMS } );
    - remove : ( ( cast_remove )db.manipulate[ action_remove ] )( &db, &a_i, act_on_{TABEL}, { ID_INREGISTRARE } );
    - update : ( ( cast_update )db.manipulate[ action_update ] )( &db, act_on_{TABEL}, { ID_INREGISTRARE }, { PARAMS } );
    - query : {TABEL} *var = ( ( cast_query )db.manipulate[ action_query ] )( &db, act_on_{TABEL}, { ID_INREGISTRARE } ).query_{TABEL};
    - query_ex : db_query_extended var = ( ( cast_query_ex )db.manipulate[ action_query_ex ] )( &db, &a_i, act_on_{TABEL|, { PARAMS } );
    - sort : ( ( cast_sort )db.manipulate[ action_sort ] )( &db, &a_i, act_on_{TABEL} );
    - calculate : int var = ( ( cast_calculate )db.manipulate[ action_calculate ] )( &db, &a_i, {TIP_CALCUL} );

    Pentru functia query_ex, parametrii dupa care NU se doreste realizarea cautarii sunt inlocuiti astfel:
        i) int -> _
        ii) char -> ""
*/

/* Prototypes */

void insereaza_date_test( DataBase *db, auto_increment *a_i );
void cerinta1( DataBase *db, auto_increment *a_i );
void cerinta2( DataBase *db, auto_increment *a_i );
void cerinta3( DataBase *db, auto_increment *a_i );

int main( )
{
    DataBase db;
    auto_increment a_i;

    db_initiate( &db, &a_i );

    insereaza_date_test( &db, &a_i );

    cerinta1( &db, &a_i );
    cerinta2( &db, &a_i );
    cerinta3( &db, &a_i );

    db_free_mem( &db );

    return 0;
}

/* C E R I N T E */

void insereaza_date_test( DataBase *db, auto_increment *a_i )
{
    ( ( cast_insert )db->manipulate[ action_insert ] )( &*db, &*a_i, act_on_modele, 1, 6, 2500, "Stratocaster", "Fender", "standard" );
    ( ( cast_insert )db->manipulate[ action_insert ] )( &*db, &*a_i, act_on_modele, 1, 12, 4230, "Les Paul", "Gibson", "standard" );
    ( ( cast_insert )db->manipulate[ action_insert ] )( &*db, &*a_i, act_on_modele, 2, 18, 3400, "Telecaster", "Fender", "drop c" );
    ( ( cast_insert )db->manipulate[ action_insert ] )( &*db, &*a_i, act_on_modele, 2, 24, 2850, "Jaguar", "Fender", "drop g" );
    ( ( cast_insert )db->manipulate[ action_insert ] )( &*db, &*a_i, act_on_modele, 1, 30, 1500, "Iceman", "Ibanez", "standard" );

    ( ( cast_insert )db->manipulate[ action_insert ] )( &*db, &*a_i, act_on_stock, 7, 2, 40 );
    ( ( cast_insert )db->manipulate[ action_insert ] )( &*db, &*a_i, act_on_stock, 5, 0, 56 );
    ( ( cast_insert )db->manipulate[ action_insert ] )( &*db, &*a_i, act_on_stock, 5, 2, 0 );
    ( ( cast_insert )db->manipulate[ action_insert ] )( &*db, &*a_i, act_on_stock, 7, 3, 15 );

    ( ( cast_insert )db->manipulate[ action_insert ] )( &*db, &*a_i, act_on_magazine, 0, 123, "Str. Academiei Nr. 20" );
    ( ( cast_insert )db->manipulate[ action_insert ] )( &*db, &*a_i, act_on_magazine, 0, 123, "Str. Restantelor fara Nr." ); // gluma subtila
    ( ( cast_insert )db->manipulate[ action_insert ] )( &*db, &*a_i, act_on_magazine, 1, 123, "Str. Grivita Nr. 18" );
    ( ( cast_insert )db->manipulate[ action_insert ] )( &*db, &*a_i, act_on_magazine, 0, 123, "Str. Nestiintei Nr. 20" );
    ( ( cast_insert )db->manipulate[ action_insert ] )( &*db, &*a_i, act_on_magazine, 1, 123, "Str. Minunilor Nr. 14" );

    ( ( cast_insert )db->manipulate[ action_insert ] )( &*db, &*a_i, act_on_patroni, 2, "Adi", "Pislaru", "0723056422" );
    ( ( cast_insert )db->manipulate[ action_insert ] )( &*db, &*a_i, act_on_patroni, 4, "Adi", "Pislaru", "0723056422" );

    ( ( cast_insert )db->manipulate[ action_insert ] )( &*db, &*a_i, act_on_facturi, 2, 1, 123, 1 );
    ( ( cast_insert )db->manipulate[ action_insert ] )( &*db, &*a_i, act_on_facturi, 2, 1, 123, 0 );
    ( ( cast_insert )db->manipulate[ action_insert ] )( &*db, &*a_i, act_on_facturi, 2, 1, 123, 1 );
    ( ( cast_insert )db->manipulate[ action_insert ] )( &*db, &*a_i, act_on_facturi, 2, 1, 123, 1 );
    ( ( cast_insert )db->manipulate[ action_insert ] )( &*db, &*a_i, act_on_facturi, 2, 1, 123, 1 );
    ( ( cast_insert )db->manipulate[ action_insert ] )( &*db, &*a_i, act_on_facturi, 2, 1, 123, 1 );
    ( ( cast_insert )db->manipulate[ action_insert ] )( &*db, &*a_i, act_on_facturi, 4, 1, 123, 1 );
    ( ( cast_insert )db->manipulate[ action_insert ] )( &*db, &*a_i, act_on_facturi, 4, 1, 123, 1 );
    ( ( cast_insert )db->manipulate[ action_insert ] )( &*db, &*a_i, act_on_facturi, 4, 1, 123, 1 );
    ( ( cast_insert )db->manipulate[ action_insert ] )( &*db, &*a_i, act_on_facturi, 4, 1, 123, 1 );
}

void cerinta1( DataBase *db, auto_increment *a_i )
{
    printf( "[Cerinta 1]\nSa se afiseze toate chitarele ce au acordaj tip: 'standard'.\n\n" );

    int i;

    db_query_extended cauta_chitare = ( ( cast_query_ex )db->manipulate[ action_query_ex ] )( &*db, &*a_i, act_on_modele, _, _, _, "", "", "standard" );

    if( cauta_chitare.num == 0 )
        printf( "Nu am gasit chitare de acest model !" );
    else
    {
        for( i = 0; i < cauta_chitare.num; i++ )
        {
            modele *chitara = ( ( cast_query )db->manipulate[ action_query ] )( &*db, act_on_modele, cauta_chitare.vars[ i ] ).query_modele;

            printf( "Id: %d | Tip: %d | Corzi: %d | Pret: %d RON | Nume: %s | Brand: %s | Acordaj: %s\n", chitara->id, chitara->tip, chitara->corzi, chitara->pret, chitara->nume, chitara->brand, chitara->acordaj );
        }
    }

    printf( "\n\n" );
}

void cerinta2( DataBase *db, auto_increment *a_i )
{

    printf( "[Cerinta 2]\nSa se introduca de la tastatura numele si prenumele unui patron,\n\tsi sa se afiseze adresa magazinului cel mai profitabil pe care il detine.\n\n" );

    int i, idmax = -1, maxi = 0;

    char nume[ STR_MAX_LOW ], prenume[ STR_MAX_LOW ];

    printf( "Nume: " );
    scanf( "%s", &nume );

    printf( "Prenume: " );
    scanf( "%s", &prenume );

    printf( "\n" );

    db_query_extended cauta_patron = ( ( cast_query_ex )db->manipulate[ action_query_ex ] )( &*db, &*a_i, act_on_patroni, _, nume, prenume, "" );

    if( cauta_patron.num == 0 )
        printf( "Nu am gasit magazine detinute de acest patron !\n\n\n" );
    else
    {
        for( i = 0; i < cauta_patron.num; i++ )
        {
            patroni *ptr = ( ( cast_query )db->manipulate[ action_query ] )( &*db, act_on_patroni, cauta_patron.vars[ i ] ).query_patroni;

            db_query_extended cauta_mag = ( ( cast_query_ex )db->manipulate[ action_query_ex ] )( &*db, &*a_i, act_on_facturi, ptr->id_magazin, _, _, 1 );

            if( cauta_mag.num > maxi )
            {
                maxi = cauta_mag.num;
                idmax = ptr->id_magazin;
            }
        }

        magazine *mgz = ( ( cast_query )db->manipulate[ action_query ] )( &*db, act_on_magazine, idmax ).query_magazine;

        if( idmax != _ )
            printf( "Adresa: %s\n\n\n", mgz->adresa );
        else
            printf( "Date insuficiente !\n\n\n" );
    }
}

void cerinta3( DataBase *db, auto_increment *a_i )
{
    printf( "[Cerinta 3]\nSa se verifice disponibilitatea unui Brand de chitara in magazinele din baza de date.\n\n" );

    int i, j, ok = 0;

    char brand[ STR_MAX ];

    printf( "Brand: " );
    scanf( "%s", &brand );

    printf( "\n" );

    db_query_extended cauta_chitare2 = ( ( cast_query_ex )db->manipulate[ action_query_ex ] )( &*db, &*a_i, act_on_modele, _, _, _, "", brand, "" );

    if( cauta_chitare2.num == 0 )
        printf( "Nu am gasit date in baza de date !" );
    else
    {
        for( i = 0; i < cauta_chitare2.num; i++ )
        {
            modele *chitara = ( ( cast_query )db->manipulate[ action_query ] )( &*db, act_on_modele, cauta_chitare2.vars[ i ] ).query_modele;

            db_query_extended cauta_in_stock = ( ( cast_query_ex )db->manipulate[ action_query_ex ] )( &*db, &*a_i, act_on_stock, _, chitara->id, _ );

            for( j = 0; j < cauta_in_stock.num; j++ )
            {
                stock *stk = ( ( cast_query )db->manipulate[ action_query ] )( &*db, act_on_stock, cauta_in_stock.vars[ j ] ).query_stock;

                if( stk->numar > 0 )
                {
                    printf( "Id Model: %d | Nume: %s | Brand: %s | Bucati: %d | Id Magazin: %d\n", stk->id_model, chitara->nume, chitara->brand, stk->numar, stk->id_magazin );

                    ok = 1;
                }
            }
        }

        if( !ok )
            printf( "Nu am gasit brand-ul in stock !" );
    }

    printf( "\n\n\n" );
}

/* S F A R S I T */
