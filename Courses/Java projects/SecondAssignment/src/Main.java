import java.io.*;
import java.util.Scanner;

public class Main {

    private static void CheckFile(File file, int typeIO) throws IOException {

        // If typeIO == 1 => Input file
        // If typeIO == 2 => Output file

        if (!file.exists()) {
            throw new IOException("Wrong output file!");
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
        Scanner scan = new Scanner(System.in);
        System.out.print("Enter input file: ");
        String inputFileName = scan.nextLine();

        File inputFile = new File(inputFileName);
        try {
            CheckFile(inputFile, 1);
        } catch (IOException err) {
            System.out.println(err.getMessage());
            return;
        }

        System.out.print("Enter output file: ");
        String outputFileName = scan.next();

        File ouputFile = new File(outputFileName);
        try {
            CheckFile(ouputFile, 2);
        } catch (IOException err) {
            System.out.println(err.getMessage());
            return;
        }

        try {
            int[] numberOfLetters = new int[52];

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
            fileReaderBuffered.close();
            fileReader.close();

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
            fileWriterBuffered.close();
            fileWriter.close();
        } catch (IOException err) {
            System.out.println("IOException: " + err.getMessage());
        }

        System.out.println("Letters counted.");
    }
}