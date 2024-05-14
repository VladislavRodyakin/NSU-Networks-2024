import os
import wikipediaapi

try:
    os.mkdir('./downloaded')
except FileExistsError:
    pass

storage_path = os.path.abspath('./downloaded')
wikiapi = wikipediaapi.Wikipedia('myproject','eo')
categ_page = wikiapi.page('Kategorio:Geografio')
cat_mem = categ_page.categorymembers

pages_total = len(cat_mem)
pages_count = 0
pages_skipped = 0

for page in cat_mem.values():
    try:
        with open(f'{storage_path}/{'_'.join(page.title.split(":"))}.txt', 'w', encoding='utf-8') as file:
            file.write(page.text)
        pages_count += 1
        print(f'Successful download, {pages_count} out of {pages_total} done. Page downloaded: {page.title}')
    except Exception as err:
        print(f'Error with page {page.title}: {err}. Skipping...')
        pages_skipped += 1


print(f'Pages downloaded: {pages_count} out of {pages_total}')
print(f'Pages skipped: {pages_skipped}')

