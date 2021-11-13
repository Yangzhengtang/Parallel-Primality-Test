//  curving using API from https://antv-g2.gitee.io/en/examples/line/basic#curved

import { Chart } from '@antv/g2';

const data = [
{Start: 32, Time: 93100, Impl: 'Par-16'}, {Start: 33, Time: 335600, Impl: 'Par-16'}, {Start: 34, Time: 440200, Impl: 'Par-16'}, {Start: 35, Time: 617100, Impl: 'Par-16'}, {Start: 36, Time: 563900, Impl: 'Par-16'}, {Start: 37, Time: 651200, Impl: 'Par-16'}, {Start: 38, Time: 522800, Impl: 'Par-16'}, {Start: 39, Time: 803900, Impl: 'Par-16'}, {Start: 40, Time: 739400, Impl: 'Par-16'}, {Start: 41, Time: 725000, Impl: 'Par-16'}, {Start: 42, Time: 733600, Impl: 'Par-16'}, {Start: 43, Time: 695500, Impl: 'Par-16'}, {Start: 44, Time: 750300, Impl: 'Par-16'}, {Start: 45, Time: 882100, Impl: 'Par-16'}, {Start: 46, Time: 824400, Impl: 'Par-16'}, {Start: 47, Time: 618000, Impl: 'Par-16'}, {Start: 48, Time: 944400, Impl: 'Par-16'}, {Start: 49, Time: 737400, Impl: 'Par-16'}, {Start: 50, Time: 797400, Impl: 'Par-16'}, {Start: 51, Time: 718600, Impl: 'Par-16'}, {Start: 52, Time: 817500, Impl: 'Par-16'}, {Start: 53, Time: 718000, Impl: 'Par-16'}, {Start: 54, Time: 846300, Impl: 'Par-16'}, {Start: 55, Time: 835800, Impl: 'Par-16'}, {Start: 56, Time: 743900, Impl: 'Par-16'}, {Start: 57, Time: 1052900, Impl: 'Par-16'}, {Start: 58, Time: 828400, Impl: 'Par-16'}, {Start: 59, Time: 708400, Impl: 'Par-16'}, {Start: 60, Time: 913800, Impl: 'Par-16'}, {Start: 61, Time: 1105100, Impl: 'Par-16'}, {Start: 62, Time: 1199200, Impl: 'Par-16'}, {Start: 63, Time: 1013700, Impl: 'Par-16'},{Start: 32, Time: 83300, Impl: 'Par-8'}, {Start: 33, Time: 317400, Impl: 'Par-8'}, {Start: 34, Time: 410600, Impl: 'Par-8'}, {Start: 35, Time: 608600, Impl: 'Par-8'}, {Start: 36, Time: 543100, Impl: 'Par-8'}, {Start: 37, Time: 660100, Impl: 'Par-8'}, {Start: 38, Time: 507300, Impl: 'Par-8'}, {Start: 39, Time: 759700, Impl: 'Par-8'}, {Start: 40, Time: 702200, Impl: 'Par-8'}, {Start: 41, Time: 743400, Impl: 'Par-8'}, {Start: 42, Time: 692300, Impl: 'Par-8'}, {Start: 43, Time: 679300, Impl: 'Par-8'}, {Start: 44, Time: 728900, Impl: 'Par-8'}, {Start: 45, Time: 846600, Impl: 'Par-8'}, {Start: 46, Time: 799200, Impl: 'Par-8'}, {Start: 47, Time: 610500, Impl: 'Par-8'}, {Start: 48, Time: 912900, Impl: 'Par-8'}, {Start: 49, Time: 721900, Impl: 'Par-8'}, {Start: 50, Time: 746200, Impl: 'Par-8'}, {Start: 51, Time: 711000, Impl: 'Par-8'}, {Start: 52, Time: 762700, Impl: 'Par-8'}, {Start: 53, Time: 723800, Impl: 'Par-8'}, {Start: 54, Time: 771500, Impl: 'Par-8'}, {Start: 55, Time: 766300, Impl: 'Par-8'}, {Start: 56, Time: 721600, Impl: 'Par-8'}, {Start: 57, Time: 966900, Impl: 'Par-8'}, {Start: 58, Time: 831900, Impl: 'Par-8'}, {Start: 59, Time: 646500, Impl: 'Par-8'}, {Start: 60, Time: 846000, Impl: 'Par-8'}, {Start: 61, Time: 1081600, Impl: 'Par-8'}, {Start: 62, Time: 1188500, Impl: 'Par-8'}, {Start: 63, Time: 931400, Impl: 'Par-8'},{Start: 32, Time: 75400, Impl: 'Par-4'}, {Start: 33, Time: 317300, Impl: 'Par-4'}, {Start: 34, Time: 383800, Impl: 'Par-4'}, {Start: 35, Time: 553400, Impl: 'Par-4'}, {Start: 36, Time: 507600, Impl: 'Par-4'}, {Start: 37, Time: 578200, Impl: 'Par-4'}, {Start: 38, Time: 485500, Impl: 'Par-4'}, {Start: 39, Time: 718000, Impl: 'Par-4'}, {Start: 40, Time: 658000, Impl: 'Par-4'}, {Start: 41, Time: 692200, Impl: 'Par-4'}, {Start: 42, Time: 658900, Impl: 'Par-4'}, {Start: 43, Time: 619200, Impl: 'Par-4'}, {Start: 44, Time: 672100, Impl: 'Par-4'}, {Start: 45, Time: 821700, Impl: 'Par-4'}, {Start: 46, Time: 762100, Impl: 'Par-4'}, {Start: 47, Time: 622100, Impl: 'Par-4'}, {Start: 48, Time: 823600, Impl: 'Par-4'}, {Start: 49, Time: 646300, Impl: 'Par-4'}, {Start: 50, Time: 761000, Impl: 'Par-4'}, {Start: 51, Time: 633000, Impl: 'Par-4'}, {Start: 52, Time: 720000, Impl: 'Par-4'}, {Start: 53, Time: 711600, Impl: 'Par-4'}, {Start: 54, Time: 749600, Impl: 'Par-4'}, {Start: 55, Time: 766200, Impl: 'Par-4'}, {Start: 56, Time: 775200, Impl: 'Par-4'}, {Start: 57, Time: 1010300, Impl: 'Par-4'}, {Start: 58, Time: 697600, Impl: 'Par-4'}, {Start: 59, Time: 760000, Impl: 'Par-4'}, {Start: 60, Time: 860200, Impl: 'Par-4'}, {Start: 61, Time: 979500, Impl: 'Par-4'}, {Start: 62, Time: 1076900, Impl: 'Par-4'}, {Start: 63, Time: 911400, Impl: 'Par-4'},{Start: 32, Time: 60900, Impl: 'Par-2'}, {Start: 33, Time: 320700, Impl: 'Par-2'}, {Start: 34, Time: 396500, Impl: 'Par-2'}, {Start: 35, Time: 552500, Impl: 'Par-2'}, {Start: 36, Time: 500600, Impl: 'Par-2'}, {Start: 37, Time: 597300, Impl: 'Par-2'}, {Start: 38, Time: 490200, Impl: 'Par-2'}, {Start: 39, Time: 732600, Impl: 'Par-2'}, {Start: 40, Time: 657300, Impl: 'Par-2'}, {Start: 41, Time: 692300, Impl: 'Par-2'}, {Start: 42, Time: 640800, Impl: 'Par-2'}, {Start: 43, Time: 709000, Impl: 'Par-2'}, {Start: 44, Time: 673000, Impl: 'Par-2'}, {Start: 45, Time: 858400, Impl: 'Par-2'}, {Start: 46, Time: 769100, Impl: 'Par-2'}, {Start: 47, Time: 612200, Impl: 'Par-2'}, {Start: 48, Time: 825300, Impl: 'Par-2'}, {Start: 49, Time: 732800, Impl: 'Par-2'}, {Start: 50, Time: 884400, Impl: 'Par-2'}, {Start: 51, Time: 710500, Impl: 'Par-2'}, {Start: 52, Time: 711500, Impl: 'Par-2'}, {Start: 53, Time: 694700, Impl: 'Par-2'}, {Start: 54, Time: 732200, Impl: 'Par-2'}, {Start: 55, Time: 745400, Impl: 'Par-2'}, {Start: 56, Time: 854300, Impl: 'Par-2'}, {Start: 57, Time: 1027400, Impl: 'Par-2'}, {Start: 58, Time: 794400, Impl: 'Par-2'}, {Start: 59, Time: 692000, Impl: 'Par-2'}, {Start: 60, Time: 839800, Impl: 'Par-2'}, {Start: 61, Time: 1097300, Impl: 'Par-2'}, {Start: 62, Time: 1208500, Impl: 'Par-2'}, {Start: 63, Time: 962200, Impl: 'Par-2'},{Start: 32, Time: 60000, Impl: 'Seq'}, {Start: 33, Time: 350600, Impl: 'Seq'}, {Start: 34, Time: 433200, Impl: 'Seq'}, {Start: 35, Time: 614600, Impl: 'Seq'}, {Start: 36, Time: 548300, Impl: 'Seq'}, {Start: 37, Time: 760900, Impl: 'Seq'}, {Start: 38, Time: 588000, Impl: 'Seq'}, {Start: 39, Time: 748700, Impl: 'Seq'}, {Start: 40, Time: 731800, Impl: 'Seq'}, {Start: 41, Time: 841200, Impl: 'Seq'}, {Start: 42, Time: 724400, Impl: 'Seq'}, {Start: 43, Time: 838500, Impl: 'Seq'}, {Start: 44, Time: 702100, Impl: 'Seq'}, {Start: 45, Time: 873800, Impl: 'Seq'}, {Start: 46, Time: 845300, Impl: 'Seq'}, {Start: 47, Time: 792800, Impl: 'Seq'}, {Start: 48, Time: 914700, Impl: 'Seq'}, {Start: 49, Time: 774700, Impl: 'Seq'}, {Start: 50, Time: 972900, Impl: 'Seq'}, {Start: 51, Time: 860500, Impl: 'Seq'}, {Start: 52, Time: 755200, Impl: 'Seq'}, {Start: 53, Time: 801200, Impl: 'Seq'}, {Start: 54, Time: 748700, Impl: 'Seq'}, {Start: 55, Time: 931100, Impl: 'Seq'}, {Start: 56, Time: 970100, Impl: 'Seq'}, {Start: 57, Time: 1073900, Impl: 'Seq'}, {Start: 58, Time: 920400, Impl: 'Seq'}, {Start: 59, Time: 790600, Impl: 'Seq'}, {Start: 60, Time: 938700, Impl: 'Seq'}, {Start: 61, Time: 1101900, Impl: 'Seq'}, {Start: 62, Time: 1442700, Impl: 'Seq'}, {Start: 63, Time: 991700, Impl: 'Seq'},
];

const chart = new Chart({
  container: 'container',
  autoFit: true,
  height: 500,
});

chart.data(data);
chart.scale({
  Start: {
    range: [0, 1],
  },
  Time: {
    nice: true,
  },
});

chart.tooltip({
  showCrosshairs: true,
  shared: true,
});

chart.axis('Time', {
  label: {
    formatter: (val) => {
      return val + 'Msec';
    },
  },
});

chart
  .line()
  .position('Start*Time')
  .color('Impl')
  .shape('smooth');

chart
  .point()
  .position('Start*Time')
  .color('Impl')
  .shape('circle');

chart.render();
