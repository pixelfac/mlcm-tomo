function maxProjLen = getMaxProjection(r, os, theta, x)

    pv1 = zeros(2, length(theta));
    pv2 = zeros(2, length(theta));
    pv3 = zeros(2, length(theta));
    pv4 = zeros(2, length(theta));
    v = [x, -x, -x,  x; ...
         x,  x, -x, -x];

    p = [r*cosd(theta); r*sind(theta)];

    pv1(1, :) = v(1, 1) - p(1,:);
    pv1(2, :) = v(2, 1) - p(2,:);
    
    pv2(1, :) = v(1, 2) - p(1,:);
    pv2(2, :) = v(2, 2) - p(2,:);
    
	pv3(1, :) = v(1, 3) - p(1,:);
    pv3(2, :) = v(2, 3) - p(2,:);
    
	pv4(1, :) = v(1, 4) - p(1,:);
    pv4(2, :) = v(2, 4) - p(2,:);
    
    cosTheta = zeros(4, length(theta));
    ps = [-cosd(theta); -sind(theta)];
    cosTheta(1, :) = abs( (ps(1,:).*pv1(1,:) + ps(2,:).*pv1(2,:)) ./ sqrt( sum(pv1.^2 , 1)) );
    cosTheta(2, :) = abs( (ps(1,:).*pv2(1,:) + ps(2,:).*pv2(2,:)) ./ sqrt( sum(pv2.^2 , 1)) );
    cosTheta(3, :) = abs( (ps(1,:).*pv3(1,:) + ps(2,:).*pv3(2,:)) ./ sqrt( sum(pv3.^2 , 1)) );
    cosTheta(4, :) = abs( (ps(1,:).*pv4(1,:) + ps(2,:).*pv4(2,:)) ./ sqrt( sum(pv4.^2 , 1)) );
    
    minCosTheta = min(cosTheta(:));
    
    maxTheta = acosd(minCosTheta);
    maxProjLen = tand(maxTheta) * (r + os) * 2;
end

