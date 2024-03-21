import java.io.*;
import java.util.Scanner;

public class Main {

    // Функция проверяющая права доступа файла, а также его существование (для входного файла)
    // Если все хорошо, то она выводит "File correct.", иначе - кидает IOException, которое мы
    // обрабатываем в main.
    private static void CheckFile(File file, int typeIO) throws IOException {

        // If typeIO == 1 => Input file
        // If typeIO == 2 => Output file

        if (!file.exists()) {
            throw new IOException("Wrong input file!");
        } else if (typeIO == 1 && !file.canRead()) {
            throw new IOException("Wrong input file rights!");
        } else if (typeIO == 2 && !file.canWrite()) {
            throw new IOException("Wrong output file rights!");
        } else if (!file.isFile()) {
            throw new IOException("It is not a file...");
        }
        System.out.println("File correct.");
    }

    public static void main(String[] args) {
        // Считывание имени входного файла
        Scanner scan = new Scanner(System.in);
        System.out.print("Enter input file: ");
        String inputFileName = scan.nextLine();

        // Проверка входного файла
        File inputFile = new File(inputFileName);
        try {
            CheckFile(inputFile, 1);
        } catch (IOException err) {
            System.out.println("IOException: " + err.getMessage());
            return;
        }

        // Считывание выходного файла
        System.out.print("Enter output file: ");
        String outputFileName = scan.next();

        // Проверка выходного файла - если его нет, то он создается + Выводится "File ... was created"
        // Если он есть, то проверяются прова доступа и если что ловится исключение
        File ouputFile = new File(outputFileName);
        try {
            if (!ouputFile.createNewFile()) {
                CheckFile(ouputFile, 2);
            } else {
                System.out.println("File " + outputFileName + " was created.");
            }
        } catch (IOException err) {
            System.out.println("IOException: " + err.getMessage());
            return;
        }

        // Дальше большой блок подсчета символов.
        // Он ловит возможные IOExceptions
        try {
            // Массив с количеством элементов
            int[] numberOfLetters = new int[52];

            // В цикле читается файл и сразу считаются буквы
            FileReader fileReader = new FileReader(inputFile);
            BufferedReader fileReaderBuffered = new BufferedReader(fileReader);
            String data;
            while ((data = fileReaderBuffered.readLine()) != null) {
                for (int pos = 0; pos < data.length(); ++pos) {
                    char c = data.charAt(pos);

                    if (c >= 'A' && c <= 'Z') {
                        ++numberOfLetters[c - 'A'];
                    } else if (c >= 'a' && c <= 'z') {
                        ++numberOfLetters[c - 'a' + 26];
                    }
                }
            }
            fileReader.close();

            // Дальше мы записываем в выходной файл количество посчитанных символов,
            // я решил не выводить символы для которых количество равно нулю
            FileWriter fileWriter = new FileWriter(ouputFile);
            BufferedWriter fileWriterBuffered = new BufferedWriter(fileWriter);
            for (int pos = 0; pos < 52; ++pos) {

                if (numberOfLetters[pos] > 0) {
                    char help;
                    if (pos < 26) {
                        help = (char) (pos + 'A');
                    } else {
                        help = (char) (pos - 26 + 'a');
                    }

                    fileWriterBuffered.write(help + " " + numberOfLetters[pos] + "\n");
                }
            }
            fileWriter.close();
        } catch (IOException err) {
            System.out.println("IOException: " + err.getMessage());
            return;
        }

        // Если все завершилось хорошо, без исключений - выводится Letters counted.
        System.out.println("Letters counted.");
    }
}