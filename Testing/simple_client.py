from client import Client

requests = [
    {
        "code": 0,
        "label": "Login",
        "payload": {"username": "", "password": ""},
    },
    {
        "code": 1,
        "label": "Register",
        "payload": {"username": "", "password": "", "email": ""},
    },
    {
        "code": 10,
        "label": "Create Room",
        "payload": {
            "roomName": "",
            "maxUsers": 0,
            "questionCount": 0,
            "answerTimeout": 0,
        },
    },
    {
        "code": 15,
        "label": "Get Rooms",
        "payload": {},
    },
]

client = Client()

req = 0

for i, k in enumerate(requests):
    print(f"{i}. {k['label']}")

try:
    while True:
        req = input("Enter request: ")
        if req == "exit":
            break

        try:
            req = int(req)
        except ValueError:
            print("Invalid input")
            continue

        if not 0 <= req < len(requests):
            print("Invalid request index")
            continue

        payload = {}

        for k, v in requests[req]["payload"].items():
            value = input(f"Enter {k}: ")
            if type(v) is int:
                payload[k] = int(value)
            else:
                payload[k] = value

        code, result = client.send(requests[req]["code"], payload)

        print(f"Got response [Code: {code}]\tPayload: {result}")
except KeyboardInterrupt:
    try:
        client.send(10, {})
    except:
        pass
