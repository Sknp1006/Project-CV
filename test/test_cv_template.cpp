#include "template/cv_features.h"

int main()
{
    pcv::SurfMatcher Surf;
    cv::Mat Template = cv::imread("test.jpg");

    cv::Mat ToMatch = Template(cv::Rect(250, 250, Template.cols / 2, Template.rows / 2)).clone();
    
    pcv::SurfMatcher::SURFDATA TempData = Surf.calcSurfData(Template);
    pcv::SurfMatcher::SURFDATA MatchData = Surf.calcSurfData(ToMatch);
    Surf.trainMatcher(TempData);

    std::vector<cv::DMatch> Matches;
    cv::Mat PerspectiveMat;
    Surf.match(MatchData, TempData, Matches, PerspectiveMat);
    cv::Mat AfterPerspective;
    Surf.wrapPerspective(ToMatch, AfterPerspective, PerspectiveMat, Template.size());
    Surf.showMatchResult(TempData, Template, MatchData, ToMatch, Matches);
    cv::waitKey(0);
    return 0;
}