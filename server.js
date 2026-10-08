const express = require('express');
const fs = require('fs');
const path = require('path');

const app = express();
const PORTA = process.env.PORT || 3000;

app.use(express.json());
app.use(express.static(__dirname));

const CONFIG = path.join(__dirname, 'config.json');

app.get('/config.json', (req, res) => res.sendFile(CONFIG));

app.post('/salvar', (req, res) => {
  try {
    fs.writeFileSync(CONFIG, JSON.stringify(req.body, null, 2));
    res.json({ ok: true });
  } catch (e) {
    res.json({ ok: false });
  }
});

app.get('/', (req, res) => res.sendFile(path.join(__dirname, 'index.html')));

app.listen(PORTA, '0.0.0.0', () => console.log('✅ ONLINE!'));
