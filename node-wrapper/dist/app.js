"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const dicomanon_1 = require("./dicomanon");
const dcm = new dicomanon_1.DcmAnon(__dirname + '/../sample.dcm');
dcm.makeAnonFile(dicomanon_1.patientInfo.PatientAge | dicomanon_1.patientInfo.PatientName).save("-");
//# sourceMappingURL=app.js.map