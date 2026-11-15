#!/usr/bin/env python3
import os

def dump_project(start_path=".", output_file="alles_komplett_master.txt"):
    code_extensions = ('.pwn', '.inc', '.hpp', '.cpp', '.h', '.py', '.js', '.html', '.css', '.md', '.cfg', '.ini', '.sql', '.json', '.sh')
    
    tree_lines = []
    file_contents = []
    
    tree_lines.append(f"=== VERZEICHNISSTRUKTUR ===\n")
    tree_lines.append(f"Verzeichnis: {os.path.abspath(start_path)}\n")
    
    for root, dirs, files in os.walk(start_path):
        # Ignoriere Git-Ordner oder virtuelle Umgebungen zur Vermeidung von Ballast
        if '.git' in root or 'node_modules' in root:
            continue
            
        level = root.replace(start_path, '').count(os.sep)
        indent = '│   ' * (level - 1) + '├── ' if level > 0 else ''
        
        folder_name = os.path.basename(root)
        if folder_name:
            tree_lines.append(f"{indent}{folder_name}/")
        
        sub_indent = '│   ' * level + '├── ' if level > 0 else '├── '
        for i, file in enumerate(files):
            is_last = (i == len(files) - 1)
            current_sub_indent = sub_indent.replace('├──', '└──') if is_last and not dirs else sub_indent
            tree_lines.append(f"{current_sub_indent}{file}")
            
            # Prüfen ob es eine Code-/Textdatei ist und Inhalt auslesen
            file_path = os.path.join(root, file)
            if file.endswith(code_extensions) and os.path.isfile(file_path):
                rel_path = os.path.relpath(file_path, start_path)
                try:
                    with open(file_path, 'r', encoding='utf-8', errors='ignore') as cf:
                        content = cf.read()
                    
                    file_contents.append(f"\n{'='*80}\n")
                    file_contents.append(f"DATEI: {rel_path}\n")
                    file_contents.append(f"{'='*80}\n\n")
                    file_contents.append(content)
                    file_contents.append("\n")
                except Exception as e:
                    file_contents.append(f"\n[Fehler beim Lesen von {rel_path}: {e}]\n")

    # Alles zusammen in die Textdatei schreiben
    output_path = os.path.join(start_path, output_file)
    with open(output_path, 'w', encoding='utf-8') as out:
        out.write('\n'.join(tree_lines))
        out.write("\n\n" + "="*80 + "\n")
        out.write("=== VOLLSTÄNDIGE CODE- INHALTE ===\n")
        out.write("="*80 + "\n")
        out.write(''.join(file_contents))
        
    print(f"Baumstruktur und sämtliche Code-Inhalte erfolgreich in '{output_path}' gespeichert.")

if __name__ == "__main__":
    dump_project()
