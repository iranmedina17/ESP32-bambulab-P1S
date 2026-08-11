# GitHub Release Checklist / Lista Para Publicar En GitHub

## Espanol

Antes de subir el proyecto a GitHub:

- Verifica que `.gitignore` incluya `include/config.h`.
- No subas claves Wi-Fi.
- No subas LAN Access Code.
- No subas serial real de la impresora si no quieres hacerlo publico.
- No subas IP local real si prefieres mantenerla privada.
- Mantén `include/config.example.h` como plantilla publica.
- Si ya creaste un repositorio y accidentalmente agregaste `include/config.h`, quitelo del indice:

```bash
git rm --cached include/config.h
```

Luego confirma:

```bash
git status
```

Busca datos sensibles antes de publicar:

```bash
rg "YOUR_REAL_WIFI_NAME|YOUR_REAL_WIFI_PASSWORD|YOUR_REAL_ACCESS_CODE|YOUR_REAL_PRINTER_SERIAL|YOUR_REAL_PRINTER_IP"
```

El comando no deberia devolver secretos en archivos que vayas a subir.

## English

Before uploading the project to GitHub:

- Make sure `.gitignore` includes `include/config.h`.
- Do not upload Wi-Fi passwords.
- Do not upload the LAN Access Code.
- Do not upload the real printer serial number unless you want it public.
- Do not upload your real local IP if you prefer to keep it private.
- Keep `include/config.example.h` as the public template.
- If you already created a repository and accidentally added `include/config.h`, remove it from the index:

```bash
git rm --cached include/config.h
```

Then confirm:

```bash
git status
```

Search for sensitive data before publishing:

```bash
rg "YOUR_REAL_WIFI_NAME|YOUR_REAL_WIFI_PASSWORD|YOUR_REAL_ACCESS_CODE|YOUR_REAL_PRINTER_SERIAL|YOUR_REAL_PRINTER_IP"
```

The command should not return secrets in files you plan to upload.
