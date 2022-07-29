(function ($) {
    //declare function
    $.fn.maj_data = function (selector, libre, occupe) {
        var chart = $(selector).highcharts();
        chart.series[0].setData([libre]);
        chart.series[1].setData([occupe]);
    };
})(jQuery);
