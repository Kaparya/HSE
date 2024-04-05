import java.io.CharConversionException;
import java.util.Calendar;
import java.util.GregorianCalendar;
import java.util.Scanner;
import java.util.Date;

public class Main {

    // Класс для хранения Даты
    static class OurDate {
        int day = 0;
        int month = 0;
        int year = 0;
    }

    // Функция проверки правильности ввода ФИО (должны быть только буквы)
    // Иначе бросается исключение CharConversionException.
    static void CheckName(String value) throws CharConversionException {
        for (int i = 0; i < value.length(); ++i) {
            if (!Character.isAlphabetic(value.charAt(i))) {
                throw new CharConversionException("Wrong character in input! (Not letter maybe)");
            }
        }
    }

    // Функция которая парсит дату, а также проверяет ее правильность.
    // В случае ошибки выбрасывается исключение NumberFormatException
    static OurDate ParseDate(String string) throws NumberFormatException {
        // Разделим строку по символу '.'
        String[] parsed_string = string.split("\\.");

        Main.OurDate result = new Main.OurDate();
        if (parsed_string.length != 3) {
            // Если дата была введена не через точку или просто неправильно - выбрасывается исключение
            throw new NumberFormatException("Wrong date format!");
        }

        // Сохраняем дату
        result.day = Integer.parseInt(parsed_string[0]);
        result.month = Integer.parseInt(parsed_string[1]);
        result.year = Integer.parseInt(parsed_string[2]);

        // Проверка на правильность - дней в месяце, год високосные или нет и т.д.
        if (result.day < 1 || result.month < 1 || result.month > 12) {
            throw new NumberFormatException("Date numbers out of bounds!");
        } else if (result.month == 2) {
            // February 28 / 29 days
            if (result.year % 4 == 0 && result.day > 29) {
                throw new NumberFormatException("There are only 29 days in february that year!");
            } else if (result.year % 4 != 0 && result.day > 28) {
                throw new NumberFormatException("There are only 28 days in february that year!");
            }
        } else if (result.month <= 7) {
            if (result.month % 2 == 1 && result.day > 31) {
                throw new NumberFormatException("There are only 31 days in that month!");
            } else if (result.month % 2 == 0 && result.day > 30) {
                throw new NumberFormatException("There are only 30 days in that month!");
            }
        } else {
            if (result.month % 2 == 0 && result.day > 31) {
                throw new NumberFormatException("There are only 31 days in that month!");
            } else if (result.month % 2 == 1 && result.day > 30) {
                throw new NumberFormatException("There are only 30 days in that month!");
            }
        }

        return result;
    }

    // Функция, которая выводит пол человека (здесь самый простой способ - нам ФИО приходит
    // из русских букв => заметим, что мужские отчества заканчиваются на 'ч', соответственно
    // будем так и проверять.
    static String GetSex(String patronymic) {
        if (patronymic.charAt(patronymic.length() - 1) == 'ч') {
            return "Мужской";
        }
        return "Женский";
    }

    // Функция подсчета возраста исходя из текущей даты.
    static int CountAge(OurDate dateOfBirth) throws NumberFormatException {
        // Получение текущей даты
        Date now = new Date();
        Calendar now_calendar = new GregorianCalendar();
        now_calendar.setTime(now);
        OurDate current = new OurDate();
        current.year = now_calendar.get(Calendar.YEAR);
        current.month = now_calendar.get(Calendar.MONTH) + 1;
        current.day = now_calendar.get(Calendar.DAY_OF_MONTH);

        // Вывод возраста
        int age = current.year - dateOfBirth.year;
        if (current.month < dateOfBirth.month) {
            age -= 1;
        } else if (current.month == dateOfBirth.month && current.day < dateOfBirth.day) {
            age -= 1;
        }

        // Если возраст отрицательный - дата неправильная
        if (age < 0) {
            throw new NumberFormatException("Date of birth is in the future! Unable to count age.");
        }

        return age;
    }

    public static void main(String[] args) {
        Scanner scan = new Scanner(System.in);
        System.out.print("Введите фамилию: ");
        String surname = scan.nextLine();
        System.out.print("Введите имя: ");
        String name = scan.nextLine();
        System.out.print("Введите отчество: ");
        String patronymic = scan.nextLine();
        // Проверка ФИО
        try {
            CheckName(surname);
            CheckName(name);
            CheckName(patronymic);
        } catch (CharConversionException exception) {
            System.out.println("\nEXCEPTION: " + exception.getMessage());
            return;
        }

        System.out.print("Введите дату рождения в формате ДД.ММ.ГГГГ: ");
        String OurDate_string = scan.nextLine();
        OurDate dateOfBirth;
        // Проверка даты
        try {
            dateOfBirth = ParseDate(OurDate_string);
        } catch (NumberFormatException exception) {
            System.out.println("\nEXCEPTION: " + exception.getMessage());
            return;
        }

        // Ввод пола
        String sex = GetSex(patronymic);

        // Проверка возраста
        int age;
        try {
            age = CountAge(dateOfBirth);
        } catch (NumberFormatException exception) {
            System.out.println("\nEXCEPTION: " + exception.getMessage());
            return;
        }

        System.out.println("====== Results ======");
        System.out.println("ФИО:     " + surname + " " + name.charAt(0) + "." + patronymic.charAt(0) + ".");
        System.out.println("Пол:     " + sex);
        System.out.println("Возраст: " + age);
    }
}