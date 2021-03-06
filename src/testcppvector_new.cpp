// testcppvector.cpp
//
// by Haoran Xiao, Mar 26, 2018
//
//---------------------------------------------------------
#include <string>
#include <iostream>
#include <cstdint>
#include <vector>

#include "geostar.hpp"

#include "boost/filesystem.hpp"

#include "IFile.hpp"

int main() {

    // delete output file if already exists
    boost::filesystem::path p("testcpp.h5");
    boost::filesystem::remove(p);
    


	// test vector creation, opening, closing
	GeoStar::File * file = new GeoStar::File("testcpp.h5","new");
	GeoStar::Vector * vec = file->create_vector("vector1", "vector1 description");
	delete vec;
	try {
		vec = file->create_vector("vector1", "vector1 duplicate description");
	}
	catch(...) {
		std::cout << "Caught exception as expected: 1" << std::endl;
	}
	vec = file->open_vector("vector1");
	delete vec;

        std::cout<<"main: 1"<<std::endl;

	// test shape creation, opening, closing
	vec = file->open_vector("vector2");

        std::cout<<"main: 2====================================="<<std::endl;

	GeoStar::Shape * shape = vec->create_shape("shape1", "shape 1 description");



        std::cout<<"main: 3==================================="<<std::endl;



	delete shape;

        std::cout<<"main: 3.1====================================="<<std::endl;

	try {
		shape = vec->create_shape("shape1", "shape 1 description");
	}
	catch(...) {
		std::cout << "Caught exception as expected: 2" << std::endl;
	}
        std::cout<<"main: 3.2====================================="<<std::endl;

	shape = vec->open_shape("shape1");
        std::cout<<"main: 3.2.0.2====================================="<<std::endl;
        shape->addPoint(13.5, 26.9);
        shape->addPoint(413.5, 826.9);
        SimpleFeatures::Point<double> aa(45.8, 129.45);
        shape->addPoint(aa);

        //SimpleFeatures::Point3D<double> aa3d(4566.8, 129.45, 94.12);
        //shape->addPoint(aa3d);


        SimpleFeatures::Line<double> ab;
        ab.resize(3);
        ab[0]=aa;
        aa.x=37.8; aa.y=12.54;
        ab[1]=aa;
        aa.x=307.8; aa.y=112.54;
        ab[2]=aa;
        shape->addLine(ab);
        std::cout<<"main: 3.2.1====================================="<<std::endl;

        SimpleFeatures::Ring<double> ac(ab);   // ring guarantees the "line" is closed (1st pt = last pt)
        std::cout<<"main: 3.2.1.1====================================="<<std::endl;
        SimpleFeatures::Polygon<double> adf;
        adf.push_back(ac);  // FIRST IS ALWAYS THE OUTER RING!
        adf.push_back(ac);  //  an inner ring
        std::cout<<"main: 3.2.2====================================="<<std::endl;
        shape->addPolygon(adf);
        std::cout<<"main: 3.2.3====================================="<<std::endl;
        

        shape->getGeometries();
	delete shape;
        std::cout<<"main: 3.2.5====================================="<<std::endl;

	shape = vec->open_shape("shape1");
        std::cout<<"main: 3.2.6====================================="<<std::endl;
	delete shape;
        std::cout<<"main: 3.2.7====================================="<<std::endl;

        std::cout<<"===========testing within======================="<<std::endl;
        shape = vec->create_shape("shape10", "shape for testing within");
	shape->addPoint(0.5, 0.5);
	shape->addPoint(4, 4);
	shape->addPoint(100, 375);

        SimpleFeatures::Line<double> aab;
        aab.resize(5);
	SimpleFeatures::Point<double> aaa(0, 0);
        aab[0]=aaa;
        aaa.x=1; aaa.y=0;
        aab[1]=aaa;
	aaa.x=1; aaa.y=1;
        aab[2]=aaa;
	aaa.x=0; aaa.y=1;
        aab[3]=aaa;
	aaa.x=0; aaa.y=0;
        aab[4]=aaa;
        shape->addLine(aab);
	SimpleFeatures::Ring<double> aac(aab);
	SimpleFeatures::Polygon<double> aaf;
	aaf.push_back(aac);
	shape->addPolygon(aaf);
	
	std::string polygonText = "POLYGON((0 0, 0 1, 1 1, 1 0, 0 0))";
	std::vector<std::pair<int, std::string>> res = shape->getWithin(polygonText);
	std::cout<<"======begin outputting result from getWithin===="<<std::endl;
	for (int i = 0; i < res.size(); ++i) {
	    std::cout << "getWithin::PK: " << res[i].first << ", geoText: " << res[i].second << std::endl;
	}
	
	std::cout<<"===========testing length======================="<<std::endl;
        shape = vec->create_shape("shape11", "shape for testing length");
	shape->addPoint(1, 1);
	SimpleFeatures::Line<double> line;
        line.resize(3);
	SimpleFeatures::Point<double> pt(0, 0);
        line[0]=pt;
        pt.x=1; pt.y=0;
        line[1]=pt;
	pt.x=1; pt.y=1;
        line[2]=pt;
	shape->addLine(line);
	std::cout<<"======begin outputting result from getLength===="<<std::endl;
	std::vector<std::pair<int, double>> res2 = shape->getLength();
	for (size_t i = 0; i < res2.size(); ++i) {
	    std::cout << "getLength::PK: " << res2[i].first << ", geoLength: " << res2[i].second << std::endl;
	}

	std::cout<<"===========testing perimeter======================="<<std::endl;
        shape = vec->create_shape("shape12", "shape for testing perimeter");
	shape->addPoint(10, 5);
	SimpleFeatures::Line<double> line2;
        line2.resize(3);
	SimpleFeatures::Point<double> pt2(0, 0);
        line2[0]=pt2;
        pt2.x=2; pt2.y=0;
        line2[1]=pt2;
	pt2.x=1; pt2.y=1;
        line2[2]=pt2;
	
	shape->addLine(line2);
        SimpleFeatures::Ring<double> ring(line2);
	SimpleFeatures::Polygon<double> polygon;
	polygon.push_back(ring);
	shape->addPolygon(polygon);

	std::cout<<"======begin outputting result from getPeremeter===="<<std::endl;
	std::vector<std::pair<int, double>> res3 = shape->getPerimeter();
	for (size_t i = 0; i < res3.size(); ++i) {
	    std::cout << "getPeremeter::PK: " << res3[i].first << ", geoPeremeter: " << res3[i].second << std::endl;
	}

	std::cout<<"======begin outputting result from getArea======"<<std::endl;
	std::vector<std::pair<int, double>> res4 = shape->getArea();
	for (size_t i = 0; i < res4.size(); ++i) {
	    std::cout << "getArea::PK: " << res4[i].first << ", getArea: " << res4[i].second << std::endl;
	}

	std::cout<<"======begin outputting result from getBoundary======"<<std::endl;
	std::vector<std::pair<int, std::string>> res5 = shape->getBoundary();
	for (size_t i = 0; i < res5.size(); ++i) {
	    std::cout << "getBoundary::PK: " << res5[i].first << ", getBoundary: " << res5[i].second << std::endl;
	}

	std::cout<<"======begin outputting result from getEnvelope======"<<std::endl;
	std::vector<std::pair<int, std::string>> res6 = shape->getEnvelope();
	for (size_t i = 0; i < res6.size(); ++i) {
	    std::cout << "getEnvelope::PK: " << res6[i].first << ", getEnvelope: " << res6[i].second << std::endl;
	}

	std::cout<<"======begin outputting result from getExpand======"<<std::endl;
	std::vector<std::pair<int, std::string>> res7 = shape->getExpand();
	for (size_t i = 0; i < res7.size(); ++i) {
	    std::cout << "getExpand::PK: " << res7[i].first << ", getExpand: " << res7[i].second << std::endl;
	}

	std::cout<<"======begin outputting result from getCentroid======"<<std::endl;
	std::vector<std::pair<int, std::string>> res8 = shape->getCentroid();
	for (size_t i = 0; i < res8.size(); ++i) {
	    std::cout << "getCentroid::PK: " << res8[i].first << ", getCentroid: " << res8[i].second << std::endl;
	}
        


	std::cout<<"===========testing simplify======================="<<std::endl;
	shape = vec->create_shape("shape13", "shape for testing simplify");
	shape->addPoint(3.3, 5);
	SimpleFeatures::Line<double> line3;
	line3.resize(4);
	SimpleFeatures::Point<double> pt3(0, 0);
	line3[0]=pt3;
        pt3.x=1; pt3.y=0.01;
	line3[1]=pt3;
        pt3.x=2; pt3.y=0;
        line3[2]=pt3;
	pt3.x=1; pt3.y=1;
        line3[3]=pt3;
	shape->addLine(line3);
	
	std::cout<<"======begin outputting result from getSimplify======"<<std::endl;
	std::vector<std::pair<int, std::string>> res9 = shape->getSimplify();
	for (size_t i = 0; i < res9.size(); ++i) {
	    std::cout << "getSimplify::PK: " << res9[i].first << ", getSimplify: " << res9[i].second << std::endl;
	}

	exit(-1);//debug

        std::cout<<"main: 3.3====================================="<<std::endl;
	shape = vec->open_shape("shape2");
	delete shape;
        std::cout<<"main: 3.4====================================="<<std::endl;
	vec->delete_shape("shape1");
	vec->delete_shape("shape2");
        std::cout<<"main: 3.5====================================="<<std::endl;


#ifdef NOTNOW
        // print all the srid's
	shape = vec->open_shape("shape1");


    { // scope the query, so it is deleted at end of scope.
      SQLite::Statement   query(*(shape->db), "SELECT * FROM spatial_ref_sys;");

      while (query.executeStep())
        {
          const char *srid   = query.getColumn(0);
          const char *name   = query.getColumn(3);
          const char *text1   = query.getColumn(4);
          const char *text2   = query.getColumn(5);
          
          std::cout  << srid << ", " << name << ", |" << text1<<"|" << std::endl;
        }//end-while
    } //end of scope for query.

	vec->delete_shape("shape1");
#endif // NOTNOW




	delete vec;


        std::cout<<"main: DONE"<<std::endl;
        exit(-1);



















        std::cout<<"main: 4==========================="<<std::endl;

	// test shape import

	try{ 
          vec = file->open_vector("vector1");
        }
        catch(...) {
          std::cout << "Failed vector open" << std::endl;
          exit(-1);
	}
       
   
#ifdef CRAP
        std::cout<<"main: 4.5: about to read_shape==========================="<<std::endl;

	//try {
		shape = vec->read_shape("reg2001_s.shp", "reg2001");
                //}
                //	catch(...) {
                //		shape = nullptr;
                //		std::cout << "Failed vector import" << std::endl;
                //         exit(-1);
                //	}
	delete shape;

        std::cout<<"main: 5==========================="<<std::endl;
        exit(-1); // for now.

	// test get_column_as_string
	shape = vec->open_shape("reg2001");
        std::vector<std::string> tmp = shape->get_column_as_string("REGIONE");
	for (auto & i : tmp)
		std::cout << i << std::endl;
	tmp = shape->get_column_as_string("ST_AREA(geom)");
	for (auto & i : tmp)
		std::cout << i << std::endl;


        std::cout<<"main: 6============================"<<std::endl;

	// test get_geoms_as_WKT
        std::vector<std::string> tmp2 = shape->get_geoms_as_WKT();

	// I would suggest piping the output to a file if you want to print this out
	// it gets quite long for even just 20 shapes
	for (auto & i : tmp2)
		std::cout << i << std::endl;
	delete shape;
#endif

	delete vec;
	delete file;
}// end-main