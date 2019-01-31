phonebook = {
    "John" : 126579,
    "Jack" : 651687,
    "Jill" : 351687
}

phonebook["Jake"] = 65461654
phonebook.pop("Jill")

if "Jake" in phonebook: 
    print("Jake is listed in the phonebook.")
if "Jill" not in phonebook:
    print("Jill is not listed in the phonebook.")   
