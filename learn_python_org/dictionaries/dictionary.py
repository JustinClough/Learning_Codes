phonebook = {}
phonebook["John"] = 35063546315
phonebook["Jack"] = 13853547874
phonebook["Jill"] = 18534516879
print(phonebook)

phonebook_2 = {
    "john" : 102930128,
    "jack" : 129312391,
    "jill" : 120331119
}
print( phonebook_2)

for name, number in phonebook.items():
    print("Phone number of %s is %d" % (name,number))

del phonebook_2["john"]
print(phonebook_2)

phonebook.pop("John")
print( phonebook)
