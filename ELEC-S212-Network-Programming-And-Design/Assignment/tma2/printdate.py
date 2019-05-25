try_again = False

while try_again ==False:
    try:
        date_string = input("Please enter a date (Year Month Day): ")
        date_list = date_string.split(' ')

        year = date_list[0]
        month = date_list[1]
        day = date_list[2]

        if month == '01' or month =='1':
            month = 'January'
        if month == '02' or month =='2':
            month = 'February'
        if month == '03' or month =='3':
            month = 'March'
        if month == '04' or month =='4':
            month = 'April'
        if month == '05' or month =='5':
            month = 'May'
        if month == '06' or month =='6':
            month = 'June'
        if month == '07' or month =='7':
            month = 'July'
        if month == '08' or month =='8':
            month = 'August'
        if month == '09' or month =='9':
            month = 'September'
        if month == '10':
            month = 'October'
        if month == '11':
            month = 'November'
        if month == '12':
            month = 'December'


        if year.isalpha():
            print("year must be a number.")
            continue

        if day.isalpha():
            print("day must be a number.")
            continue

        if day > "31":
            print("invalid day")
            continue

        if month == 'a':
            print("month must be a number")
            continue

        print(month, day, ',', year)
        try_again == True


    except IndexError as error:
        print("Usage: print date year month day")
        continue





