
def my_function():
    print("Hello from my function!")

def my_function_with_args( username, greetings):
    print("Hello, %s, from my function!. I wish you %s" %(username, greetings))

def sum_two_numbers( a, b):
    return a+b

my_function()

my_function_with_args( "John", "a great year!")

x = sum_two_numbers( 1,2)
