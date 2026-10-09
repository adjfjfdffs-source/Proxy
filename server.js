#include "esp.h"
const express = require('express');
const fs = require('fs');
const path = require('path');

const app = express();
const PORTA = process.env.PORT || 3000;

app.use(express.json());
app.use(express.static(__dirname));

const CONFIG = path.join(__dirname, 'config.json');

if (!fs.existsSync(CONFIG)) {
  fs.writeFileSync(CONFIG, JSON.stringify({
    bypass: false,
    aim_assist: false,
    precision_disfarcado: false,
    sensi_alta: false,
    hs_pescoco: false,
    back_jump: false
  }, null, 2));
}

// ==========================================
// 🔒 COLA O SEU CÓDIGO QUE VOCÊ SALVOU AQUI ⬇️
// ==========================================
const CODIGOS_ATIVACAO = {
  bypass: `
    // BYPASS • Proteção antiban
    function AtivarBypass() {
      console.log("✅ BYPASS ATIVADO");
    }
    // COLA SEU CÓDIGO REAL AQUI
  `,
  aim_assist: `
    // AIM ASSIST
  `,
  precision_disfarcado: `
    // PRECISION DISFARÇADO
  `,
  sensi_alta: `
    // SENSI ALTA
  `,
  hs_pescoco: `
    // HS PESCOÇO
  `,
  back_jump: `
    // BACK JUMP
  `
};
// ==========================================
// 🔒 FIM — Seu código tá seguro aqui
// ==========================================

app.get('/config.json', (req, res) => {
  res.sendFile(CONFIG);
});

app.post('/salvar', (req, res) => {
  fs.writeFileSync(CONFIG, JSON.stringify(req.body, null, 2));
  res.json({ok: true});
});

app.listen(PORTA, () => {
  console.log(`✅ Rodando na porta ${PORTA}`);
});
