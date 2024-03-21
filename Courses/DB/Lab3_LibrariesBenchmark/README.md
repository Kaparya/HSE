# Лабораторная работа №3 Benchmark

### Описание лабораторной работы

Задание данной лабораторной работы заключалось в написании бенчмарка для измерения скорости выполения 4 запросов на 5 различных библиотеках на языке Python (psycopg2, SQLite, DuckDB, Pandas, SQLAlchemy).

### Задача

Была поставлена задача проверить и сравнить время работы 4 запросов из статьи на [Medium](https://medium.unum.cloud/pandas-cudf-modin-arrow-spark-and-a-billion-taxi-rides-f85973bfafd5):

* First query
  ```sql
  SELECT cab_type, count(*)
  FROM trips
  GROUP BY 1;
  ```
* Second query
  ```sql
  SELECT passenger_count, avg(total_amount) 
  FROM trips 
  GROUP BY 1;
  ```
* Third query
  ```sql
  SELECT
     passenger_count, 
     extract(year from pickup_datetime),
     count(*)
  FROM trips
  GROUP BY 1, 2;
  ```
* Fourth query
  ```sql
  SELECT
      passenger_count,
      extract(year from pickup_datetime),
      round(trip_distance),
      count(*)
  FROM trips
  GROUP BY 1, 2, 3
  ORDER BY 2, 4 desc;
  ```

На 4 библиотеках языка Python:
1. Pandas
2. DuckDB
3. Postgres
4. SQLite
5. SQLAlchemy

### Ход работы
Весь код был написан на языке Python с использованием pgAdmin4 и DB Browser for SQLite. Тесты проводились на платформе Apple Macbook Air m1 (16 gb / 512 gb). Все файлы с кодом находятся в репозитории, так же для запуска потребуются файлы с базой данных (.csv and .db), GitHub не позволяет загрузить из-за размера, поэтому они на [Диске](https://disk.yandex.ru/d/qFi1xFNMSjERJg). Для запуска эти файлы необходимо разместить в той же папке, где main.


---
0. Все функции для проверок библиотек лежат в одной папке - Operation_files, называются "CheckИмяБиблиотеки" и имеют одинаковую структуру: они принимают количество повторений тестов (tries) и флаг big_data (если он false => используется файл tiny, если true => big).
   В функции создаются массивы со значениями суммарного времени работы, и времени работы каждого query.
   Функция возвращает массив посчитанных медианных значений по массивам суммарного времени работы, и времени работы каждого query.

    **Пример файла для библиотеки Pandas**
      ```python
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
      ```


---
1. Сначала были написаны все функции для всех библиотек, они располагаются в папке Operation_files.

   **Впечатления от работы с библиотеками:**

   * Pandas - неприятная реализация SQL запросов (нельзя просто написать SQL запросы). Однако, считывание базы данных из .csv очень удобно и понятно (возможность хранения в Датафрейме)
  
   * DuckDB - очень удобно и очень просто: есть возможность считать базу данных из .csv одной функцией, запросы делаются обычным SQL языком.
  
   * Psycopg - требовалось подключиться к базе данных в postgres, неприятно. Query пишутся на обычном SQL.
  
   * SQLite - нужно было создать .db файл и подключиться к нему - неприятно. Не все функции из SQL работали, потребовалось найти аналоги для работы с SQLite - очень неприятно, когда не знаешь про это, а ошибка есть (описание ошибки ужасно).
  
   * SQLAlchemy - подключение к postgres - неприятно. Других проблем не было.
  
   * Python - впервые использовал серьезно, до этого чисто алгоритмические задачи. В принципе прикольно, но плюсы лучше (just kidding).

---
2. Результаты тестов.

   Представлены медианные значения по 28 запускам для файла tiny и по 17 запускам для файла big.

   - **Pandas**

     <img width="500" alt="Screenshot 2023-12-17 at 23 25 36" src="https://github.com/Kaparya/DB_benchmark/assets/124422354/d8498dbe-d6f3-4461-921b-68e1ae32dd2f">
     <img width="500" alt="Screenshot 2023-12-17 at 23 26 52" src="https://github.com/Kaparya/DB_benchmark/assets/124422354/733ec34a-bf7c-4b89-8bb4-c32f99c01c07">
   
   - **DuckDB**

     <img width="500" alt="Screenshot 2023-12-17 at 23 33 16" src="https://github.com/Kaparya/DB_benchmark/assets/124422354/439bc5f0-541f-4a4d-95f8-747d8ed7c88b">
     <img width="500" alt="Screenshot 2023-12-17 at 23 33 32" src="https://github.com/Kaparya/DB_benchmark/assets/124422354/b7e0670d-e739-4528-996a-ceffadde5854">

   - **Postgres**

     <img width="500" alt="Screenshot 2023-12-17 at 23 39 28" src="https://github.com/Kaparya/DB_benchmark/assets/124422354/0e3d3cc4-cecf-434b-a14a-9ebdba630429">
     <img width="500" alt="Screenshot 2023-12-17 at 23 40 01" src="https://github.com/Kaparya/DB_benchmark/assets/124422354/c9ef6aea-a214-4bb6-aa05-7767b8935cef">

   - **SQLite**
  
     <img width="500" alt="Screenshot 2023-12-17 at 23 45 13" src="https://github.com/Kaparya/DB_benchmark/assets/124422354/3d40187d-d323-4b3f-8ac8-7deaa658c61e">
     <img width="500" alt="Screenshot 2023-12-17 at 23 45 26" src="https://github.com/Kaparya/DB_benchmark/assets/124422354/9aeb7079-ca3b-4833-9195-fdf36adbb51c">

   - **SQLAlchemy**
  
     <img width="500" alt="Screenshot 2023-12-17 at 23 54 52" src="https://github.com/Kaparya/DB_benchmark/assets/124422354/03983578-6e90-4fe8-a3de-04adba9f5df0">
     <img width="500" alt="Screenshot 2023-12-17 at 23 56 17" src="https://github.com/Kaparya/DB_benchmark/assets/124422354/f2ef72f5-5cbe-43bc-8ec8-6d557b072bf4">

   ---
   **Сравнение библиотек**
   
   <img width="800" alt="Screenshot 2023-12-18 at 00 02 55" src="https://github.com/Kaparya/DB_benchmark/assets/124422354/5bcdda96-1141-47a3-a57b-96fc5d3570cf">

   По графику сразу заметно, что наибольшее время работу у SQLite, затем идет Psycopg, Pandas, DuckDB и SQLAlchemy. При этом, время SQLite больше всех остальных примерно в 9 раз. Однако, на первом query postgres обгоняет по времени даже DuckDB.

   <img width="800" alt="Screenshot 2023-12-18 at 00 03 05" src="https://github.com/Kaparya/DB_benchmark/assets/124422354/3d050a13-cd7c-47f3-a471-d82e8b3dbf40">

   Ситуация, не сильно отличается от tiny data (в принципе, очевидно). SQLAlchemy и тут работает настолько быстро, что его даже не видно.

---
3. Причины различия во времени

   Самая быстрая библиотека - *SQLAlchemy*. Cкорей всего именно из-за скорости часто используется в бэкенде. (По факту, должна быть медленее SQLite, из-за того, что это надстройка над ней, однако у меня она работает космически быстро, осмелюсь предпроложить, что это возможно за счет процессоров от Apple, они гораздо быстрее, чем обычные).
   
   *DuckDB* тоже быстро:
   
       * держит базу данных в памяти
   
       * хорошо оптимизирована для сложных queries
   
       * написана на C++, а не на C (в отличие от SQLite) => присутствуют оптимизации, появившиеся в C++
   
   *SQLite* медленно:
   
       * написана на C - не хватает современных оптимизаций
   
       * вышла в 2000 году

       * больше оптимизирована для работы и записи сразу в несколько БД
   
   *Postrgres* - приходится каждый раз читать данные, потому что база данных не в памяти, а только курсор по ней бегает.
   
   *Pandas* - тоже надстройка над SQLite, позволяющая хранить базу данных в датафрейме => работает быстрее, чем SQLite.

---
---
### Как пользоваться проектом?

  0. Установка

     0.1. Склонировать репозиторий к себе на компьютер
        
     0.2. Скачать файлы с базой данных (.csv and .db files) с [Диска](https://disk.yandex.ru/d/qFi1xFNMSjERJg)
        
     0.3. Положить скачанные файлы в папку рядом с main.py

  1. Запуск

     Запустить файл main.py. Появится выбор действий:

     ```python
     ------------ Benchmark ------------
     1. Run full test.
     2. Run test with tiny file.
     3. Run test with big file.
    
     4. Run test with only one library.
    
     0. Exit.
     Input the number to run (1/2/3/4/0): 
     ```

     1. Full test - полный тест и tiny, и big файла всех библиотек.
        
          1.1. После выбора этого пункта требуется ввести количество тестов для tiny файла, которые Вы хотите провести (число от 1 до 50 включительно, большее количество занимает очень много времени, однако лимит можно убрать).
  
          1.2. Сначала происходит тест для tiny файла.
  
          1.3. После тесты выводятся результаты и есть возможно сохранить их в файл (Results/ResultsOfTinyTest.txt).
  
          1.4. Затем, Вы вводите количество тестов для big файла (число от 1 до 20 включительно, большее количество занимает очень много времени, однако лимит можно убрать).
  
          1.5. Проходят тесты для big файла и опять есть возможность сохранить в файл (Results/ResultsOfBigTest.txt).

     2. Test with tiny file - тест только tiny файла всех библиотек.
    
          1.1. После выбора этого пункта требуется ввести количество тестов, которые Вы хотите провести (число от 1 до 50 включительно, большее количество занимает очень много времени, однако лимит можно убрать).
  
          1.2. Проходят тесты и выводятся результаты, есть возможно сохранить их в файл (Results/ResultsOfTinyTest.txt).

     3. Test with big file - тест только big файла всех библиотек.
    
          1.1. После выбора этого пункта требуется ввести количество тестов, которые Вы хотите провести (число от 1 до 20 включительно, большее количество занимает очень много времени, однако лимит можно убрать).
  
          1.2. Проходят тесты и выводятся результаты, есть возможно сохранить их в файл (Results/ResultsOfBigTest.txt).

     4. Test with only one library - тест только одной библиотеки.
    
          1.1. После выбора этого пункта требуется выбрать библиотеку, которую Вы хотите протестировать.
    
          ```python
          
          ```
  
          1.2. Проходят тесты и выводятся результаты, есть возможно сохранить их в файл (Results/ResultsOfBigTest.txt).
