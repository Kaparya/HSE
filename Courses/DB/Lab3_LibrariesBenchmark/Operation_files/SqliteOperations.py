import sqlite3
from time import time
from statistics import median


def SqliteFirstQuery(cursor, current_table, prints):
    query = f"""SELECT vendorid, count(*) 
                   FROM {current_table} 
                   GROUP BY 1;"""
    rows = cursor.execute(query).fetchall()

    if prints:
        for r in rows:
            print(r)


def SqliteSecondQuery(cursor, current_table, prints):
    query = f"""SELECT passenger_count, avg(total_amount) 
                   FROM {current_table}
                   GROUP BY 1;"""
    rows = cursor.execute(query).fetchall()

    if prints:
        for r in rows:
            print(r)


def SqliteThirdQuery(cursor, current_table, prints):
    query = f"""SELECT
                    passenger_count, 
                    strftime('%Y', tpep_pickup_datetime) AS "Year",
                    count(*)
                FROM {current_table}
                GROUP BY 1, 2;"""
    rows = cursor.execute(query).fetchall()

    if prints:
        for r in rows:
            print(r)


def SqliteFourthQuery(cursor, current_table, prints):
    query = f"""SELECT
                    passenger_count,
                    strftime('%Y', tpep_pickup_datetime) AS "Year",
                    round(trip_distance),
                    count(*)
                FROM {current_table}
                GROUP BY 1, 2, 3
                ORDER BY 2, 4 desc;"""
    rows = cursor.execute(query).fetchall()

    if prints:
        for r in rows:
            print(r)


def CheckSqlite(tries, big_data=False):
    prints = 0

    connection = sqlite3.connect("nyc_yellow_tiny.db")
    cursor = connection.cursor()

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
        SqliteFirstQuery(cursor, current_table, prints)
        time_first.append(time() - start_time)
        time1 = time()
        SqliteSecondQuery(cursor, current_table, prints)
        time_second.append(time() - time1)
        time2 = time()
        SqliteThirdQuery(cursor, current_table, prints)
        time_third.append(time() - time2)
        time3 = time()
        SqliteFourthQuery(cursor, current_table, prints)
        time_fourth.append(time() - time3)
        finish_time = time()

        time_sum.append(finish_time - start_time)

    connection.commit()
    connection.close()

    return [median(time_sum),
            median(time_first),
            median(time_second),
            median(time_third),
            median(time_fourth)]
