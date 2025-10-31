import os
import re

# Dossiers
src_dir = "src"
include_dir = "include"
donotouch_dir = os.path.join(include_dir, "DONOTTOUCH")
output_file = os.path.join("src", "AllFunction.h")
main_file = os.path.join(src_dir, "main.cpp")

# Exclusions
excluded_files = {"web_content.h"}
excluded_functions = {"loop", "setup", "hexToNeopixelbus"}

# Regex
function_regex = re.compile(r"^\s*([a-zA-Z_][\w\s\*]+)\s+([a-zA-Z_]\w*)\s*\(([^)]*)\)\s*\{")
include_regex = re.compile(r'^\s*#include\s*[<"]([^">]+)[">]')

# --- Étape 1 : récupérer les includes valides dans main.cpp
included_headers = set()
with open(main_file, "r", encoding="utf-8") as main:
    for line in main:
        if line.strip().startswith("//"):  # ligne commentée
            continue
        match = include_regex.match(line)
        if match:
            included_headers.add(match.group(1))  # ex: "LED_RGB.h" ou "DONOTTOUCH/setupSPIFFS.h"

print("Headers utilisés :", included_headers)

# --- Étape 2 : extraire uniquement les fonctions des fichiers inclus
functions_by_file = {}

def extract_functions_from_file(file_path, header_key):
    with open(file_path, "r", encoding="utf-8") as file:
        for line in file:
            match = function_regex.match(line)
            if match:
                return_type, func_name, params = match.groups()
                if func_name in excluded_functions:
                    continue
                if func_name in {"if", "else"}:
                    continue
                prototype = f"{return_type} {func_name}({params});"
                if header_key not in functions_by_file:
                    functions_by_file[header_key] = set()
                functions_by_file[header_key].add(prototype)

def extract_from_dir(directory, relative_prefix=""):
    for root, _, files in os.walk(directory):
        for filename in files:
            if filename in excluded_files:
                continue
            if filename.endswith(".h") or filename.endswith(".cpp"):
                # Construit le chemin relatif pour comparer avec ceux du main.cpp
                rel_path = os.path.relpath(os.path.join(root, filename), include_dir)
                rel_path = rel_path.replace("\\", "/")  # normalisation Windows/Linux

                if rel_path not in included_headers and filename not in included_headers:
                    continue  # pas inclus → on skip

                file_path = os.path.join(root, filename)
                extract_functions_from_file(file_path, rel_path)

# Extraction (src + include + DONOTTOUCH)
extract_from_dir(include_dir)
extract_from_dir(donotouch_dir, "DONOTTOUCH")
extract_from_dir(src_dir)

# --- Étape 3 : générer le fichier
with open(output_file, "w", encoding="utf-8") as out_file:
    out_file.write("// Fichier généré automatiquement\n\n")
    for filename, functions in functions_by_file.items():
        out_file.write(f"// {filename}\n")
        for function in sorted(functions):  # tri pour ordre stable
            out_file.write(f"    {function}\n")
        out_file.write("\n")

print(f"Fichier {output_file} mis à jour avec {sum(len(f) for f in functions_by_file.values())} fonctions.")
