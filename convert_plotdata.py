f = open('./plot-curve')
csv = 'time\told_cov\tnew_cov\n'
hour = 0
for line in f:
    line_items = line.split(' ')
    # print(line_items)
    time = line_items[0]
    if line_items[21] == str(hour) + "h":
        old_cov = line_items[3][0:line_items[3].index('(')]
        new_cov = line_items[12][3:line_items[12].index('(')]
        # old_cov = line_items[3][0:line_items[3].index('(')]
        # new_cov = line_items[12]
        csv += str(hour) + '\t' + old_cov + '\t' + new_cov + '\n'
        # csv += time + ',' + old_cov + ',' + new_cov + '\n'
        hour = hour + 1

print(csv)
f = open("plot-curve-by-hour", "w")
f.write(csv)
f.close()
