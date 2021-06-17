x = QWGov2Trec09;
dim = size(x,1)
QWGov2Trec09_ids = [];
ids = [];
for i=1:dim
    
    ids = [ids;i];
end
QWGov2Trec09_ids = [ids,x];

x = queriesCW09SizeQ;
dim = size(x,1)
QWCWTrec09_ids = [];
ids = [];
for i=1:dim
    
    ids = [ids;i];
end
QWCWTrec09_ids = [ids,x];