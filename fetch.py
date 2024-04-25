import requests
from bs4 import BeautifulSoup
import sys
import json
import csv

def read_titles_from_csv(csv_file):
    titles = []
    with open(csv_file, newline='', encoding='utf-8') as file:
        reader = csv.DictReader(file)
        for row in reader:
            titles.append(row['TITLE'])
    return titles, len(titles)

def find_entry(name):
    url = "https://umu.openwinecomponents.org/index.php"

    data = {
        "search": name
    }

    try:
        response = requests.post(url, data=data)

        if response.status_code == 200:
            soup = BeautifulSoup(response.text, "html.parser")

            results_table = soup.find("table", class_="results")

            rows = results_table.find_all("tr")[1:]

            entries = []

            for row in rows:

                cells = row.find_all("td")

                entry = {
                    "Number": cells[0].get_text(),
                    "Title": cells[1].get_text(),
                    "UMU ID": cells[2].get_text(),
                    "Store": cells[3].get_text(),
                    "Notes": cells[6].get_text(),
                    "Protonfixes Script": cells[7].find("a")["href"] if cells[7].find("a") else ""
                }

                entries.append(entry)

            return json.dumps(entries)
        else:
            print("Error: ", response.status_code)
            return json.dumps({"error": f"Error: {response.status_code}"})
    except requests.RequestException as e:
        print("Error: ", e)

def main():
    if len(sys.argv) == 2:
        if sys.argv[1] == "--help":
            print("Usage: python fetch.py --test <file.csv")
            print("Usage: python fetch.py <search_query>")
            return
        search_query = sys.argv[1]
        result = find_entry(search_query)
        print(result)
    elif len(sys.argv) > 2:
        if sys.argv[1] == "--test":
            csv_file = sys.argv[2]
            titles, total_entries = read_titles_from_csv(csv_file)
            for i, title in enumerate(titles, start=1):
                result = find_entry(title.strip())
                print(f"Processing entry {i}/{total_entries}")
        else:
            print("Usage: python fetch.py --test <file.csv")
    else:
        print(json.dumps({"error": "Usage: python fetch.py <search_query>"}))


if __name__ == "__main__":
    main()
