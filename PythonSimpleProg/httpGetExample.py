import httplib
connection = httplib.HTTPConnection("www.google.com")
connection.request("GET", "/index.html")
response = connection.getresponse()
print response.status, response.reason

pageData = response.read()
print pageData

connection.close()

