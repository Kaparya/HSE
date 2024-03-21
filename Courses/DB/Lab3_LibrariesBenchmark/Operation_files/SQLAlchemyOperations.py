import sqlalchemy
from sqlalchemy import MetaData
from sqlalchemy.sql import text
from time import time
from statistics import median


def SQLAlchemyFirstQuery(connection, current_table, prints):
    statement = text(f"""SELECT vendorid, count(*) FROM {current_table} GROUP BY 1;""")

    if prints:
        for r in connection.execute(statement):
            print(r)


def SQLAlchemySecondQuery(connection, current_table, prints):
    statement = text(f"""SELECT passenger_count, avg(total_amount) 
                         FROM {current_table} 
                         GROUP BY 1;""")

    if prints:
        for r in connection.execute(statement):
            print(r)


def SQLAlchemyThirdQuery(connection, current_table, prints):
    statement = text(f"""SELECT
                               passenger_count, 
                               extract(year from tpep_pickup_datetime),
                               count(*)
                         FROM {current_table}
                         GROUP BY 1, 2;""")

    if prints:
        for r in connection.execute(statement):
            print(r)


def SQLAlchemyFourthQuery(connection, current_table, prints):
    statement = text(f"""SELECT
                              passenger_count,
                              extract(year from tpep_pickup_datetime),
                              round(trip_distance),
                              count(*)
                         FROM {current_table}
                         GROUP BY 1, 2, 3
                         ORDER BY 2, 4 desc;""")

    if prints:
        for r in connection.execute(statement):
            print(r)


def CheckSQLAlchemy(tries, big_data=False):
    prints = 0

    engine = sqlalchemy.create_engine('postgresql://postgres:123@localhost/nyc_yellow_tiny')
    metadata = MetaData()
    metadata.create_all(engine)

    if big_data:
        current_table = "nyc_yellow_big"
    else:
        current_table = "nyc_yellow_tiny"

    time_sum = []
    time_first = []
    time_second = []
    time_third = []
    time_fourth = []

    with engine.connect() as connection:

        for i in range(tries):

            start_time = time()
            SQLAlchemyFirstQuery(connection, current_table, prints)
            time_first.append(time() - start_time)
            time1 = time()
            SQLAlchemySecondQuery(connection, current_table, prints)
            time_second.append(time() - time1)
            time2 = time()
            SQLAlchemyThirdQuery(connection, current_table, prints)
            time_third.append(time() - time2)
            time3 = time()
            SQLAlchemyFourthQuery(connection, current_table, prints)
            time_fourth.append(time() - time3)
            finish_time = time()

            time_sum.append(finish_time - start_time)

    return [median(time_sum),
            median(time_first),
            median(time_second),
            median(time_third),
            median(time_fourth)]
