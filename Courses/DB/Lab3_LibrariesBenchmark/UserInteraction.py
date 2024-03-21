from os import mkdir, path

from Operation_files.PandasOperations import CheckPandas
from Operation_files.DuckDBOperations import CheckDuckDB
from Operation_files.PsycopgOperations import CheckPostgres
from Operation_files.SqliteOperations import CheckSqlite
from Operation_files.SQLAlchemyOperations import CheckSQLAlchemy

def PrintResults(results, library):
    print(f'------------ {library} ------------')
    print('Median time of all 4 queries: {:.7f}'.format(results[0]))
    print('Median time of first query:   {:.7f}'.format(results[1]))
    print('Median time of second query:  {:.7f}'.format(results[2]))
    print('Median time of third query:   {:.7f}'.format(results[3]))
    print('Median time of fourth query:  {:.7f}\n'.format(results[4]))


def CheckLibraries(tries, big_data):
    print('/-------------\\')
    time_pandas = CheckPandas(tries, big_data)
    print('###', end='')
    time_duckdb = CheckDuckDB(tries, big_data)
    print('###', end='')
    time_psycopg = CheckPostgres(tries, big_data)
    print('###', end='')
    time_sqlite = CheckSqlite(tries, big_data)
    print('###', end='')
    time_SQLAlchemy = CheckSQLAlchemy(tries, big_data)
    print('###\n')
    return time_pandas, time_duckdb, time_psycopg, time_sqlite, time_SQLAlchemy


def SaveResults(time_pandas, time_duckdb, time_psycopg, time_sqlite, time_SQLAlchemy, big_data):
    save = input("\nWould you like to save the results? (Y): ")
    if save == 'Y':
        if not path.isdir('Results'):
            mkdir('Results')
        if big_data:
            file_output = open("Results/ResultsOfBigTest.txt", "w")
        else:
            file_output = open("Results/ResultsOfTinyTest.txt", "w")
        data = f'''--- Results ---
Pandas: 
{' '.join(map(lambda a: '{:.7f}'.format(a), time_pandas))}
DuckDB: 
{' '.join(map(lambda a: '{:.7f}'.format(a), time_duckdb))}
Postgres: 
{' '.join(map(lambda a: '{:.7f}'.format(a), time_psycopg))}
SQLite: 
{' '.join(map(lambda a: '{:.7f}'.format(a), time_sqlite))}
SQLAlchemy: 
{' '.join(map(lambda a: '{:.7f}'.format(a), time_SQLAlchemy))}'''
        print(data, file=file_output)

        print('\nData saved to ', end='')
        if big_data:
            print("Results/ResultsOfBigTest.txt")
        else:
            print("Results/ResultsOfTinyTest.txt")
        file_output.close()


def RunTest(tries, big_data):
    (time_pandas, time_duckdb, time_psycopg, time_sqlite, time_SQLAlchemy) = CheckLibraries(tries, big_data)
    PrintResults(time_pandas, 'Pandas')
    PrintResults(time_duckdb, 'DuckDB')
    PrintResults(time_psycopg, 'Postgres')
    PrintResults(time_sqlite, 'SQLite')
    PrintResults(time_SQLAlchemy, 'SQLAlchemy')

    SaveResults(time_pandas, time_duckdb, time_psycopg, time_sqlite, time_SQLAlchemy, big_data)


def RunTestTiny():
    big_data = False
    tries = -1
    while tries == -1:
        tries_str = input('Input the number of tests (no more than 50 tests): ')
        if tries_str.isnumeric() and 1 <= int(tries_str) <= 50:
            tries = int(tries_str)
            break
        print('Write correct number!\n')

    print('\n\n\nRunning test with tiny file.')
    RunTest(tries, big_data)


def RunTestBig():
    big_data = True
    tries = -1
    while tries == -1:
        tries_str = input('Input the number of tests (no more than 20 tests): ')
        if tries_str.isnumeric() and 1 <= int(tries_str) <= 20:
            tries = int(tries_str)
            break
        print('Write correct number!\n')

    print('\n\n\nRunning test with big file.')
    RunTest(tries, big_data)


def RunTestOneLibrary():
    library = -1
    while library == -1:
        print('Which library would you like to test:')
        print('0 - Pandas')
        print('1 - DuckDB')
        print('2 - Postgres')
        print('3 - SQLite')
        print('4 - SQLAlchemy')
        library_str = input('Input the number (0/1/2/3/4): ')
        if library_str.isnumeric() and 0 <= int(library_str) <= 4:
            library = int(library_str)

    big_data = -1
    while big_data == -1:
        big_data_str = input('Do you want to use tiny or big file (0 - tiny/1 - big): ')
        if big_data_str.isnumeric() and 0 <= int(big_data_str) <= 1:
            big_data = int(big_data_str)

    if library == 0:
        time_library = CheckPandas(1, big_data)
        PrintResults(time_library, 'Pandas')
    if library == 1:
        time_library = CheckDuckDB(1, big_data)
        PrintResults(time_library, 'DuckDB')
    if library == 2:
        time_library = CheckPostgres(1, big_data)
        PrintResults(time_library, 'Postgres')
    if library == 3:
        time_library = CheckSqlite(1, big_data)
        PrintResults(time_library, 'SQLite')
    if library == 4:
        time_library = CheckSQLAlchemy(1, big_data)
        PrintResults(time_library, 'SQLAlchemy')

