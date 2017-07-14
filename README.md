# procdatabase

This is a simple Procedural Programming Database Simulator, made for a Uni Homework.
I was requested to implement the basis operations to simulate a Guitar Shop.
Sorry, i was too lazy to transalte from Romanian -_-.

# Operations supported
  - Insert
  - Remove
  - Update
  - Query(ex)
  - Sort
  - Calculate
  - Free Query
  - Free Memory

# Information about structure of db
- modele : ( auto_increment id ), int tip, int corzi, int pret, char nume, char brand, char acordaj
- magazine : ( auto_increment id ), int id_patron, int data, char adresa
- stock : ( auto_increment id ), int id_magazin, int id_model, int numar
- patroni : ( auto_increment id ), int id_magazin, char nume, char prenume, char tel
- facturi : ( auto_increment id ), int id_magazin, int id_model, int data, int achitat

# Actions list
- insert
- remove
- update
- query
- query_ex
- sort
- calculate

# Actions list for 'calculate'
- minim
- maxim
- average

# List of {PARAMS}
- Listed above on Structure

# Types of actions on tables

- act_on_**{TABEL}** -> where **{TABEL}** is listed above on Structure

# Types of actions

- action_**{ACTIUNE}** -> where **{ACTIUNE}** is from the list of Actions

# Types of casting 
- cast_**{ACTIUNE}** -> where **{ACTIUNE}** is from the list of Actions

# Generic example for database manipulation

- ( ( CAST_TYPE )db.manipulate[ ACTION_TYPE ] )( { PARAMS } )

# Explicit examples for database manipulation

- insert : ( ( cast_insert )db.manipulate[ action_insert ] )( &db, &a_i, act_on_{TABEL}, { PARAMS } );
- remove : ( ( cast_remove )db.manipulate[ action_remove ] )( &db, &a_i, act_on_{TABEL}, { ID_INREGISTRARE } );
- update : ( ( cast_update )db.manipulate[ action_update ] )( &db, act_on_{TABEL}, { ID_INREGISTRARE }, { PARAMS } );
- query : {TABEL} *var = ( ( cast_query )db.manipulate[ action_query ] )( &db, act_on_{TABEL}, { ID_INREGISTRARE } ).query_{TABEL};
- query_ex : db_query_extended var = ( ( cast_query_ex )db.manipulate[ action_query_ex ] )( &db, &a_i, act_on_{TABEL|, { PARAMS } );
- sort : ( ( cast_sort )db.manipulate[ action_sort ] )( &db, &a_i, act_on_{TABEL} );
- calculate : int var = ( ( cast_calculate )db.manipulate[ action_calculate ] )( &db, &a_i, {TIP_CALCUL} );

## More examples in the main c file ##
