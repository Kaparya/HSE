import pandas as pd
from time import time
from statistics import median


def PandasFirstQuery(data, prints):
    selected_data = data[['VendorID']]
    grouped_data = selected_data.groupby('VendorID')
    final_data = grouped_data.size().reset_index(name='counts')
    if prints:
        print(final_data)


def PandasSecondQuery(data, prints):
    selected_data = data[['passenger_count', 'total_amount']]
    grouped_data = selected_data.groupby('passenger_count')
    final_data = grouped_data.mean().reset_index()
    if prints:
        print(final_data)


def PandasThirdQuery(data, prints):
    selected_data = data[['passenger_count', 'tpep_pickup_datetime']]
    selected_data['year'] = pd.to_datetime(
        selected_data.pop('tpep_pickup_datetime'),
        format='%Y-%m-%d %H:%M:%S').dt.year
    grouped_data = selected_data.groupby(['passenger_count', 'year'])
    final_data = grouped_data.size().reset_index(name='counts')
    if prints:
        print(final_data)


def PandasFourthQuery(data, prints):
    selected_data = data[['passenger_count', 'tpep_pickup_datetime', 'trip_distance']]
    selected_data['trip_distance'] = selected_data['trip_distance'].round().astype(int)
    selected_data['year'] = pd.to_datetime(
        selected_data.pop('tpep_pickup_datetime'),
        format='%Y-%m-%d %H:%M:%S').dt.year
    grouped_data = selected_data.groupby(['passenger_count', 'year', 'trip_distance'])
    final_data = grouped_data.size().reset_index(name='counts')
    final_data = final_data.sort_values(
        ['year', 'counts'],
        ascending=[True, False])
    if prints:
        print(final_data)


def CheckPandas(tries, big_data=False):
    prints = 0

    time_sum = []
    time_first = []
    time_second = []
    time_third = []
    time_fourth = []
    pd.options.mode.chained_assignment = None

    if big_data:
        data = pd.read_csv('nyc_yellow_big.csv')
    else:
        data = pd.read_csv('nyc_yellow_tiny.csv')

    for i in range(tries):

        start_time = time()
        PandasFirstQuery(data, prints)
        time_first.append(time() - start_time)
        time1 = time()
        PandasSecondQuery(data, prints)
        time_second.append(time() - time1)
        time2 = time()
        PandasThirdQuery(data, prints)
        time_third.append(time() - time2)
        time3 = time()
        PandasFourthQuery(data, prints)
        time_fourth.append(time() - time3)
        finish_time = time()

        time_sum.append(finish_time - start_time)

    return [median(time_sum),
            median(time_first),
            median(time_second),
            median(time_third),
            median(time_fourth)]
