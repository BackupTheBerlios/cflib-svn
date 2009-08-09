<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="xml" indent="yes" omit-xml-declaration="no" encoding="ISO-8859-1" standalone="yes" media-type="application/xml"/>
	
	<xsl:param name="compound">all</xsl:param>
	<xsl:param name="target">all</xsl:param>
	
	<!-- ROOT ELEMENT -->
	<xsl:template match="tagfile">
		<xsl:copy>
			<xsl:apply-templates select="@*"/>
			<xsl:choose>
				<xsl:when test="$compound=''">
					<compound kind="file">
						<name>libcf.a</name>
						<path>cflib/lib/</path>
						<filename>cflib</filename>
						<xsl:choose>
							<xsl:when test="$target='all'">
								<xsl:apply-templates select="compound[@kind!='struct']/member" mode="show"/>
							</xsl:when>
							<xsl:otherwise>
								<xsl:apply-templates select="compound[@kind!='struct']/member[@kind=$target]" mode="show"/>
							</xsl:otherwise>
						</xsl:choose>
					</compound>
				</xsl:when>
				<xsl:when test="$compound='all'">
					<xsl:apply-templates mode="showall"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:apply-templates select="compound[@kind=$compound]" mode="showall"/>
				</xsl:otherwise>
			</xsl:choose>
			<xsl:apply-templates/>
		</xsl:copy>
	</xsl:template>
	
	<!-- EXPLICIT: SHOW ELEMENT (TREE) -->
	<xsl:template match="*" mode="show">
		<xsl:copy>
			<xsl:apply-templates select="@*"/>
			<xsl:apply-templates/>
		</xsl:copy>
	</xsl:template>
	<xsl:template match="*" mode="showall">
		<xsl:copy>
			<xsl:apply-templates select="@*"/>
			<xsl:apply-templates mode="showall"/>
		</xsl:copy>
	</xsl:template>
	
	<!-- DEFAULT: HIDE KNOWN ELEMENTS -->
	<xsl:template match="compound|member"/>
	<xsl:template match="compound[@kind='group']"/>
	<xsl:template match="compound[@kind='struct']"/>
	<xsl:template match="compound[@kind='file']"/>
	<xsl:template match="compound[@kind='page']"/>
	<xsl:template match="member[@kind='define']"/>
	<xsl:template match="member[@kind='function']"/>
	<xsl:template match="member[@kind='variable']"/>
	<xsl:template match="member[@kind='typedef']"/>
	
	<!-- DEFAULT: SHOW/COPY EVERYTHING -->
	<xsl:template match="node()|@*">
		<xsl:copy>
			<xsl:apply-templates select="@*"/>
			<xsl:apply-templates/>
		</xsl:copy>
	</xsl:template>

</xsl:stylesheet>
