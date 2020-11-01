function doGet(e) {
  Logger.log( JSON.stringify(e) );
  var result = 'OK';
  if(e.parameter == 'undefined'){
    result = 'No parameters';
  } 
  else {
    var sheet_id = ''; // PUT HERE Spreadsheet ID
    var sheet = SpreadsheetApp.openById(sheet_id).getActiveSheet();
    var newRow = sheet.getLastRow() + 1;
    var rowData = [];
    var Curr_Date = new Date();
    rowData[0] = Curr_Date; // Date in column A
    var Curr_Time = Utilities.formatDate(Curr_Date, "America/Sao_Paulo", 'HH:mm:ss');
    rowData[1] = Curr_Time; //Time in column B
    for (var param in e.parameter){
      Logger.log('In for loop, param = ' + param);
      var value = e.parameters[param];
//      Logger.log('value = ' + value);
      Logger.log(param + ':' + e.parameter[param]);
      switch(param){
        case 'v_sh':
          rowData[2] = value; // Shunt Voltage[mV] in column C
          result = 'Shunt Voltage written on column C';
          break;
        case 'i_sh':
          rowData[3] = value; // Shunt Current[mA] in column D
          result += ', Shunt Current written to column D';
          break;
        case 'v_bus':
          rowData[4] = value; // Bus Voltage[V] in column E
          result = 'Bus Voltage written on column E';
          break;
        case 'p_bus':
          rowData[5] = value; // Bus Power[mW] in column F
          result += ', Bus Power written to column F';
          break;
        default:
          result = 'Unsupported parameter';
      }      
    }
    Logger.log(JSON.stringify(rowData));
    var newRange = sheet.getRange(newRow, 1, 1,rowData.length);
    newRange.setValues([rowData]);    
  }
  return ContentService.createTextOutput(result);  
}