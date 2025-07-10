# HOW TO IMPLEMENT QRIS (QR Code Indonesian Standard) ON A CLIENT-SIDE WEBSITE WITHOUT PAYMENT GATEAWAY

A WebAssembly-based QRIS (QR Code Indonesian Standard) string generator for customizing payment amounts directly in the browser.  
The core logic is written in C and compiled to WebAssembly (WASM) with JavaScript glue code, enabling fast QRIS string manipulation on the client side.



## 🚀 Features

- Generate dynamic QRIS strings with custom payment amounts
- Built in C and compiled to WebAssembly (WASM) using Emscripten
- Fully compatible with modern browsers and front-end build tools (Webpack, Vite, etc.)
- Lightweight, high-performance, and runs entirely on the client side
- No server dependency required for price customization logic

---

## 🧩 Installation

### Option 1: Install via npm

```bash
npm install npm-qris-web
```

### Option 2: Use directly via CDN

```js
import Module from 'https://unpkg.com/npm-qris-web/qris.js';
```

---

## ⚡ Usage

### Using npm (with bundler)

```js
import Module from 'npm-qris-web/qris.js';

(async () => {
  // Initialize WASM module
  const qrisModule = await Module();

  // Wrap C function
  const generate_qris = qrisModule.cwrap('generate_qris', 'string', ['string', 'number']);

  // Generate QRIS string with custom price
  const qrisBaru = generate_qris("your_base_qris_string", 25000);
  console.log("New QRIS string:", qrisBaru);
})();
```

---

### Using CDN (without install)

```html
<script type="module">
  import Module from 'https://unpkg.com/npm-qris-web/qris.js';

  const customModule = {
    locateFile(path) {
      if (path.endsWith('.wasm')) {
        return 'https://unpkg.com/npm-qris-web/qris.wasm';
      }
      return path;
    }
  };

  Module(customModule).then(qrisModule => {
    const generate_qris = qrisModule.cwrap('generate_qris', 'string', ['string', 'number']);

    const qrisBaru = generate_qris("your_base_qris_string", 25000);
    console.log("New QRIS string:", qrisBaru);
  });
</script>
```

---

## 💡 How it works

- The core logic is written in C and compiled to WebAssembly (`qris.wasm`) and JavaScript glue code (`qris.js`) using Emscripten.
- You must initialize the module by calling `Module()` before using any function.
- After initialization, you can use `cwrap` to call the C functions directly from JavaScript.

---

## 📄 API

### `generate_qris(qris_string, price)`

- **Parameters:**
  - `qris_string` (`string`): Original QRIS string you want to modify.
  - `price` (`number`): Custom price in Indonesian Rupiah.
- **Returns:**
  - `string`: New QRIS string with the updated price.

---

## 🛠️ Development & Build

If you want to build or modify:

```bash
# Clone repository
git clone https://github.com/yourusername/npm-qris-web.git
cd npm-qris-web

# Install dependencies (if any)
npm install

# Build (if using custom build steps)
# emcc your_c_file.c -o qris.js -s WASM=1 ...
```

---

## 🤝 Contributing

Contributions are welcome! Please open issues or submit pull requests.  
If you find bugs or want new features, feel free to discuss.

---

## 📄 License

MIT

---

## 💬 Contact

Author: Bwokchainkun
Email: nara@narakun.my.id

---