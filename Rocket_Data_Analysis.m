data = [];

for ii=1:length(rocketdata(:,1))
    data(ii,:) = (typecast(uint16(base2dec(rocketdata(ii,:),16)),'int16'));
end
