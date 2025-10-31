import os

def convert_files():
    site_dir = "data/site"
    
    with open("src/web_content.h", "w", encoding="utf-8") as f_out:
        f_out.write("#pragma once\n\n")
        
        for dirpath, dirnames, filenames in os.walk(site_dir):
            if 'index.html' in filenames and 'style.css' in filenames and 'script.js' in filenames:
                dir_name = os.path.basename(dirpath)
                VarName = "SiteWeb_" + dir_name
                
                files = {
                    'html': os.path.join(dirpath, 'index.html'),
                    'css': os.path.join(dirpath, 'style.css'),
                    'js': os.path.join(dirpath, 'script.js')
                }
                
                # Lire les fichiers sans modification
                with open(files['html'], 'r', encoding='utf-8') as f_html:
                    html_content = f_html.read()

                with open(files['css'], 'r', encoding='utf-8') as f_css:
                    css_content = f_css.read()

                with open(files['js'], 'r', encoding='utf-8') as f_js:
                    js_content = f_js.read()

                # Injecter CSS et JS dans le HTML sans transformation
                html_content = html_content.replace(
                    '<link rel="stylesheet" href="style.css">',
                    f'<style>\n{css_content}\n</style>'
                ).replace(
                    '<script src="script.js"></script>',
                    f'<script>\n{js_content}\n</script>'
                )

                # Générer la variable raw literal correctement
                page_content = f"const char {VarName}[] PROGMEM = R\"rawliteral(\n{html_content}\n)rawliteral\";\n\n"

                # Écrire dans le fichier de sortie
                f_out.write(page_content)

# Exécuter la conversion
convert_files()
