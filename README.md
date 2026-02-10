# cef-template

## Frontend

```
npm create vite@latest app -- --template react-ts
cd app
npm run dev
npm run build
```

* Under `Debug` mode use http://localhost:5173/
* Under `Release` mode use dist output copied to build output

  * vite.config.ts

    ```
    import { defineConfig } from 'vite'
    import react from '@vitejs/plugin-react'

    // https://vite.dev/config/
    export default defineConfig({
      plugins: [react()],
      // 使用相对路径，便于 CEF 在 file:// 协议下正确加载 dist 资源
      base: './',
    })
    ```
  * index.html

    ```
    <!doctype html>
    <html lang="en">
      <head>
        <meta charset="UTF-8" />
        <link rel="icon" type="image/svg+xml" href="./vite.svg" />
        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
        <title>app</title>
      </head>
      <body>
        <div id="root"></div>
        <script type="module" src="/src/main.tsx"></script>
      </body>
    </html>

    ```

## Backend

* Change folder name from `replace_me` to your project name
* Replace all `REPLACE_ME` and `REPLACE_ME_` with your project name
* Replace all `replace_me` and `replace_me_` with your project name
