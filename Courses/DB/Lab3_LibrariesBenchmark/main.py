from UserInteraction import RunTestBig, RunTestTiny, RunTestOneLibrary


while True:
    print('\n\n\n------------ Benchmark ------------')
    print('1. Run full test.')
    print('2. Run test with tiny file.')
    print('3. Run test with big file.\n')
    print('4. Run test with only one library.\n')
    print('0. Exit.')

    choice = -1
    while choice == -1:
        choice_str = input('Input the number to run (1/2/3/4/0): ')
        if choice_str.isnumeric() and 0 <= int(choice_str) <= 4:
            choice = int(choice_str)
            break
        print('Write correct number!\n')

    if choice == 0:
        print('\n\n\n------------ The end! ------------')
        break
    elif choice == 1:
        print('\n\nRunning test with tiny file:')
        RunTestTiny()
        print('\n\nRunning test with big file:')
        RunTestBig()
    elif choice == 2:
        RunTestTiny()
    elif choice == 3:
        RunTestBig()
    elif choice == 4:
        RunTestOneLibrary()

