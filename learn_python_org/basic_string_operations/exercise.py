s = "Str 4567aa0123 5ome!"
# Length sould be 20
print("Length of s = %d" % len(s))

#First occurrence of "a" shold be at index 8
print("The first occurence of the letter a = %d" % s.index("a"))

#Number of a's sould be at index 2
print("a occurs %d times" % s.count("a"))

# Slicing the string into bits
print("The first five characters are '%s'" %s[:5])
print("The next five characters are '%s'" %s[5:10])
print("The thirteenth character is '%s'" %s[12])
print("The character with odd index are '%s'" %s[1::2])
print("The last five characters are '%s'" %s[-5:])

print("String in uppercase: %s" % s.upper())
print("String in lowercase: %s" % s.lower())

if s.startswith("Str"):
    print("String starts with 'Str'. Good!")

if s.endswith("ome!"):
    print("String ends with 'ome!'. Good!")

print("Split the words of the string: %s" % s.split(" "))
