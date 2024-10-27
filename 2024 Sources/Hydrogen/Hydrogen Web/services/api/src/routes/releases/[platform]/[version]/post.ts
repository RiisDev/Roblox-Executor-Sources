// import { PutObjectCommand } from "@aws-sdk/client-s3";
// import { PostReleaseRoute } from "@hydrogen-web/interfaces";
// import { s3, secrets } from "@hydrogen-web/libraries";
// import { releaseModel } from "@hydrogen-web/models";
// import { domainUtil, environmentUtil } from "@hydrogen-web/utils";
// import { extname } from "node:path";
// import { server } from "../../../..";

// // todo: do this properly

// server.route<PostReleaseRoute>({
//   method: "POST",
//   url: "/releases/:platform/:version",
//   schema: {
//     params: {
//       type: "object",
//       required: ["platform", "version"],
//       properties: {
//         platform: {
//           type: "string",
//           enum: ["windows", "android", "macos", "ios"],
//         },
//         version: {
//           type: "string",
//         },
//       },
//     },
//   },
//   handler: async (request) => {
//     if (
//       !request.headers["authorization"] ||
//       request.headers.authorization !== secrets.Internal.adminApiKey
//     )
//       throw {
//         statusCode: 401,
//         message: "You do not have permission to access this resource.",
//       };

//     const data = await request.file();

//     if (!data)
//       throw {
//         statusCode: 400,
//         message: "Invalid or malformed data provided, please try again.",
//         errors: [
//           {
//             location: "file",
//             message: "Field is required.",
//           },
//         ],
//       };

//     await s3.send(
//       new PutObjectCommand({
//         Key: `${request.params.platform}/release-${
//           request.params.version
//         }${extname(data.filename)}`,
//         Body: await data.toBuffer(),
//         ContentType: data.mimetype,
//         Bucket: `hydrogen-downloads-${environmentUtil()}`,
//         ACL: "public-read",
//       })
//     );

//     const previousRelease = await releaseModel
//       .findOne({
//         platform: request.params.platform,
//       })
//       .sort({
//         createdAt: "desc",
//       });

//     if (previousRelease)
//       await s3.send(
//         new PutObjectCommand({
//           Key: `${previousRelease.platform}/release-${previousRelease.version}${previousRelease.fileType}`,
//           Bucket: `hydrogen-downloads-${environmentUtil()}`,
//           ACL: "bucket-owner-full-control",
//         })
//       );

//     const release = await releaseModel.create({
//       version: request.params.version,
//       platform: request.params.platform,
//       fileType: extname(data.filename),
//       releaseUrl: "",
//       createdAt: new Date(),
//     });

//     return {
//       data: {
//         version: release.version,
//         platform: release.platform,
//         downloadUrl: `https://releases.${domainUtil()}/${
//           release.platform
//         }/release-${release.version}${release.fileType}`,
//         createdAt: release.createdAt,
//       },
//     };
//   },
// });
