#rm my_pdf.pdf
node-gyp rebuild && node test/test.js --debug
#open -a Preview my_pdf.pdf
