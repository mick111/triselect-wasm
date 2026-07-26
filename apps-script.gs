function doGet(e) {
  try {
    const action = (e && e.parameter && e.parameter.action) ? e.parameter.action : 'get';
    const spreadsheet = SpreadsheetApp.getActiveSpreadsheet();

    if (action === 'getTeams') {
      const sheet = getOrCreateSheet(spreadsheet, 'Equipes');
      const teams = getTeamsFromSheet(sheet);
      return jsonResponse({ ok: true, teams: teams });
    }

    if (action === 'save') {
      const sheet = getOrCreateSheet(spreadsheet, 'Classement');
      const name = String(e.parameter.name || 'Anonyme').substring(0, 30).replace(/[<>]/g, '');
      const score = Math.max(0, Math.min(99999, parseInt(e.parameter.score) || 0));
      const acc = Math.max(0, Math.min(100, parseInt(e.parameter.acc) || 0));
      const team = String(e.parameter.team || '').substring(0, 50).replace(/[<>]/g, '');
      const date = new Date().toLocaleDateString('fr-FR');
      sheet.appendRow([name, score, acc, date, team]);
      return jsonResponse({ ok: true });
    }

    if (action === 'get') {
      const sheet = getOrCreateSheet(spreadsheet, 'Classement');
      const values = sheet.getDataRange().getDisplayValues();
      if (values.length <= 1) {
        return jsonResponse({ ok: true, data: [] });
      }

      const rows = values.slice(1)
        .filter(row => row[0] || row[1] || row[2] || row[4])
        .map(row => ({
          name: row[0] || '',
          score: Number(row[1] || 0),
          acc: Number(row[2] || 0),
          date: row[3] || '',
          team: String(row[4] || '').trim() || 'Sans équipe'
        }))
        .filter(item => item.team);

      const grouped = {};
      rows.forEach(row => {
        if (!grouped[row.team]) {
          grouped[row.team] = { team: row.team, scoreTotal: 0, accTotal: 0, count: 0, date: row.date, participants: [] };
        }
        grouped[row.team].scoreTotal += row.score;
        grouped[row.team].accTotal += row.acc;
        grouped[row.team].count += 1;
        grouped[row.team].date = grouped[row.team].date || row.date;
        grouped[row.team].participants.push({ name: row.name, score: row.score, acc: row.acc });
      });

      const teamRows = Object.values(grouped)
        .map(item => ({
          team: item.team,
          score: Math.round(item.scoreTotal / item.count),
          acc: Math.round(item.accTotal / item.count),
          count: item.count,
          date: item.date,
          participants: item.participants
        }))
        .sort((a, b) => b.score - a.score || b.acc - a.acc)
        .slice(0, 50);

      return jsonResponse({ ok: true, data: teamRows });
    }

    return jsonResponse({ ok: false, error: 'Action inconnue' });
  } catch (err) {
    return jsonResponse({ ok: false, error: err.message });
  }
}

function doPost(e) {
  return doGet(e);
}

function getOrCreateSheet(spreadsheet, name) {
  let sheet = spreadsheet.getSheetByName(name);
  if (!sheet) {
    sheet = spreadsheet.insertSheet(name);
    if (name === 'Equipes') {
      sheet.getRange(1, 1, 1, 1).setValue('Équipe');
      sheet.getRange(1, 1, 1, 1).setFontWeight('bold');
    }
    if (name === 'Classement') {
      sheet.getRange(1, 1, 1, 5).setValues([['Pseudo', 'Score', 'Précision (%)', 'Date', 'Équipe']]);
      sheet.getRange(1, 1, 1, 5).setFontWeight('bold');
    }
  }
  return sheet;
}

function getTeamsFromSheet(sheet) {
  const values = sheet.getDataRange().getDisplayValues();
  const teams = [];
  for (let i = 0; i < values.length; i++) {
    const team = String(values[i][0] || '').trim();
    if (!team) continue;
    if (i === 0 && /équipe/i.test(team)) continue;
    teams.push(team);
  }
  return teams;
}

function jsonResponse(obj) {
  return ContentService.createTextOutput(JSON.stringify(obj)).setMimeType(ContentService.MimeType.JSON);
}
