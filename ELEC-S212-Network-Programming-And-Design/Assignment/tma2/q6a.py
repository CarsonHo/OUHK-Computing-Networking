done_number_array = [2, 1, 37, 5, 3, 24, 1, 3, 36, 1, 2, 13, 3, 5, 26, 9, 2, 24, 8, 49, 9]

x, y, z = [int(x) for x in input("Please enter 3 keys (separted by space): ").split()]

print("The key",x,"is found",done_number_array.count(x), " times in the database")
print("The key",y,"is found",done_number_array.count(y), " times in the database")
print("The key",z,"is found",done_number_array.count(z), " times in the database")





