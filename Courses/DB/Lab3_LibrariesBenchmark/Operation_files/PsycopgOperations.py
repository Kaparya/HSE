import psycopg2
from time import time
from statistics import median


def PostgresFirstQuery(cursor, current_table, prints):
    sql = '''SELECT vendorid, count(*) FROM {} GROUP BY 1;'''.format(current_table)
    cursor.execute(sql)
    if prints:
        for i in cursor.fetchall():
            print(i)


def PostgresSecondQuery(cursor, current_table, prints):
    sql = '''SELECT passenger_count, avg(total_amount) 
             FROM {} 
             GROUP BY 1;'''.format(current_table)
    cursor.execute(sql)
    if prints:
        for i in cursor.fetchall():
            print(i)


def PostgresThirdQuery(cursor, current_table, prints):
    sql = '''SELECT
               passenger_count, 
               extract(year from tpep_pickup_datetime),
               count(*)
             FROM {}
             GROUP BY 1, 2;'''.format(current_table)
    cursor.execute(sql)
    if prints:
        for i in cursor.fetchall():
            print(i)


def PostgresFourthQuery(cursor, current_table, prints):
    sql = '''SELECT
                passenger_count,
                extract(year from tpep_pickup_datetime),
                round(trip_distance),
                count(*)
             FROM {}
             GROUP BY 1, 2, 3
             ORDER BY 2, 4 desc;'''.format(current_table)
    cursor.execute(sql)
    if prints:
        for i in cursor.fetchall():
            print(i)


def CheckPostgres(tries, big_data=False):
    prints = 0

    conn = psycopg2.connect(
        host="localhost",
        database="nyc_yellow_tiny",
        user="postgres",
        password="123")

    cursor = conn.cursor()

    if big_data:
        current_table = "nyc_yellow_big"
    else:
        current_table = "nyc_yellow_tiny"

    time_sum = []
    time_first = []
    time_second = []
    time_third = []
    time_fourth = []

    for i in range(tries):

        start_time = time()
        PostgresFirstQuery(cursor, current_table, prints)
        time_first.append(time() - start_time)
        time1 = time()
        PostgresSecondQuery(cursor, current_table, prints)
        time_second.append(time() - time1)
        time2 = time()
        PostgresThirdQuery(cursor, current_table, prints)
        time_third.append(time() - time2)
        time3 = time()
        PostgresFourthQuery(cursor, current_table, prints)
        time_fourth.append(time() - time3)
        finish_time = time()

        time_sum.append(finish_time - start_time)

    conn.commit()
    conn.close()

    return [median(time_sum),
            median(time_first),
            median(time_second),
            median(time_third),
            median(time_fourth)]
