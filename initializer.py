import pip
import requests
import sys

try:
    import sqlite3
except ImportError:
    pip.main(["install", "sqlite3"])

URL = "https://the-trivia-api.com/api/questions?categories=science,music&limit=10&region=US&difficulty=medium"


def main():
    try:
        db = sqlite3.connect(sys.argv[1])
        cursor = db.cursor()

        r = requests.get(URL)
        data = r.json()
        if r.status_code != 200:
            print("Error fetching request")
            return

        cursor.execute("DELETE FROM QUESTIONS;")

        for q in data:
            question = q["question"]
            correct = q["correctAnswer"]

            cursor.execute(
                f"INSERT INTO QUESTIONS (QUESTION, ANSWER_1, ANSWER_2, ANSWER_3, ANSWER_4, CORRECT_ANSWER_ID) VALUES (?, ?, ?, ?, ?, 0)",
                (question, correct, *q["incorrectAnswers"]),
            )

        db.commit()
    except:
        print("[-] Usage: python initializer.py <db path>")


if __name__ == "__main__":
    main()
