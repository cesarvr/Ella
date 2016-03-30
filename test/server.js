var _ = require('underscore');
var express = require('express');
var app = express();
var pdf = new require('./api/searchApi');

console.log('loading pdf in memory!');

var PDFData = function() {
    var hash = {};

    return {
        proccess: function(_hash) {
            console.log('getting lib');
            if (_.isUndefined(this.hash)) {
                hash = _hash;
            } else {
                hash = _.extend(hash, _hash);
            }

            console.log('hash->', Object.keys(hash));
        },
        getCache: function() {
            return hash;
        }
    }
};

var pdfMemCache = new PDFData();

pdf(function(api) {
    api.readAndCachePDF(pdfMemCache.proccess);
});

app.use(express.static('./test/static'));

app.get('/', function(req, res) {
    res.send('Hello World!');
});


var cacheImg = [];
app.get('/preview', function(req, res) {


    var q = req.query.pdf;
    console.log('preview->', q);
    console.log('preview ->', Object.keys(cacheImg));


    if (_.isUndefined(cacheImg[q])) {
        pdf(function(api) {
            api.previewPDF(q, function(base64) {
                //console.log('->', base64);
								cacheImg[q] = base64;
                res.send({
                    img: base64
                });
            });
        });
    } else {
        res.send({
            img: cacheImg[q]
        });

    }




});


app.get('/look', function(req, res) {

    var q = req.query.query;
    var hash = pdfMemCache.getCache();

    var hits = [];
    for (var k in hash) {

        console.log('search->', k);
        var text = hash[k];
        var pos = text.indexOf(q);
        if (pos > -1) {
            hits.push({
                file: k,
                block: text.substring((pos > 30) ? pos - 30 : pos, pos + 100)
            });
        }



    }

    res.send(hits);

});






app.listen(3000, function() {
    console.log('Serving...');
});
